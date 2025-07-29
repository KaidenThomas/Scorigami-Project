// simulateDrive.cpp
#include "simulateDrive.h" // Include its own header first
#include <iostream>
#include <random>    // For random number generation
#include <chrono>    // For seeding random number generator
#include <algorithm> // For std::min, std::max
#include <stdexcept> // For std::out_of_range in map access

// Global random engine (seeded once for the entire program run)
// This definition should only exist in ONE .cpp file.
static std::mt19937 global_rng(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

// --- SIMULATION CONSTANTS AND FORMULAS DEFINITIONS ---
// These are defined here, matching the extern declarations in simulateDrive.h
namespace SimulationConstants {
    const std::map<std::string, double> DEFENSIVE_MULTIPLIERS = {
        {"Juggernaut", 0.5},
        {"Terrifying", 0.7},
        {"Scary", 0.9},
        {"Passing", 1.1},
        {"Average", 1.0},
        {"Bad", 1.3},
        {"Terrible", 1.4}
    };

    const double ONSIDE_KICK_SUCCESS_CHANCE = 0.02;
    const double PUNT_RETURN_UNSUCCESSFUL_CHANCE = 0.005;
    const double DEFENSIVE_TD_ON_TURNOVER_CHANCE = 0.07; // 7% chance of a defensive TD on a turnover

    const int TOUCHDOWN_SUCCESSFUL_XP_POINTS = 7;
    const int TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS = 6; // TD without successful XP/2pt is 6 points
    const int FIELD_GOAL_POINTS = 3;
    const int SAFETY_POINTS = 2;

    const double DRIVE_DURATION_STD_DEV_FACTOR = 0.25;
    // Removed FIELD_POSITION_STD_DEV_FACTOR, FIELD_POSITION_MIN, FIELD_POSITION_MAX
}
// --- END SIMULATION CONSTANTS AND FORMULAS DEFINITIONS ---

// Simulates one offensive drive of a football game.
// Updates the game state based on the drive's outcome.
// Returns an updated ActiveGame instance.
ActiveGame simulateDrive(ActiveGame currentGameState) {
    // Determine offensive and defensive teams
    const Team* offenseTeam = nullptr;
    const Team* defenseTeam = nullptr;

    if (currentGameState.getPossessionTeamName() == currentGameState.getHomeTeam().getTeamName()) {
        offenseTeam = &currentGameState.getHomeTeam();
        defenseTeam = &currentGameState.getAwayTeam();
    }
    else if (currentGameState.getPossessionTeamName() == currentGameState.getAwayTeam().getTeamName()) {
        offenseTeam = &currentGameState.getAwayTeam();
        defenseTeam = &currentGameState.getHomeTeam();
    }
    else {
        std::cerr << "Error: Invalid possession team name. Cannot simulate drive." << std::endl;
        return currentGameState; // Return unchanged game state
    }

    // 1. Determine drive duration
    double driveDurationRange = offenseTeam->getLongestOffDrive() - offenseTeam->getShortestOffDrive();
    double driveDurationMean = offenseTeam->getAvgOffDrive();
    double driveDurationStdDev = driveDurationRange * SimulationConstants::DRIVE_DURATION_STD_DEV_FACTOR;

    // Ensure standard deviation is not zero for normal_distribution
    if (driveDurationStdDev == 0.0) {
        driveDurationStdDev = 0.01; // Small non-zero value to prevent issues
    }
    std::normal_distribution<double> driveDurationDist(driveDurationMean, driveDurationStdDev);
    double driveDuration = driveDurationDist(global_rng);

    // Clamp drive duration within min/max bounds
    driveDuration = std::max(offenseTeam->getShortestOffDrive(), driveDuration);
    driveDuration = std::min(offenseTeam->getLongestOffDrive(), driveDuration);

    // 2. Update clock
    HalfTimeInfo updatedHalfTimeInfo = currentGameState.getCurrentHalfTimeInfo();
    updatedHalfTimeInfo.secondsLeftInHalf -= static_cast<int>(driveDuration);

    // 3. Check for end of half due to clock
    if (updatedHalfTimeInfo.secondsLeftInHalf <= 0) {
        updatedHalfTimeInfo.secondsLeftInHalf = 0;
        std::string nextPossessionTeamName;
        if (currentGameState.didHomeTeamReceiveKickoff()) {
            nextPossessionTeamName = currentGameState.getAwayTeam().getTeamName();
        }
        else {
            nextPossessionTeamName = currentGameState.getHomeTeam().getTeamName();
        }
        currentGameState.setPossessionTeamName(nextPossessionTeamName);
        // Removed currentGameState.setCurrentFieldPosition(25.0);
        currentGameState.setCurrentHalfTimeInfo(updatedHalfTimeInfo); // Update only secondsLeftInHalf
        std::cout << "Drive ended due to clock running out. No score change (possession flips)." << std::endl;
        return currentGameState;
    }

    // 4. Determine drive outcome probabilities adjusted by defense
    std::map<std::string, double> adjustedProbabilities;
    double totalAdjustedProb = 0.0;

    // Get defensive multiplier
    double defensiveMultiplier = 1.0;
    try {
        defensiveMultiplier = SimulationConstants::DEFENSIVE_MULTIPLIERS.at(defenseTeam->getDefensiveRating());
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: Defensive rating '" << defenseTeam->getDefensiveRating() << "' not found in multipliers map. " << e.what() << ". Using default multiplier 1.0." << std::endl;
    }

    // Adjust positive outcomes: better defense reduces offensive success (multiplier < 1.0)
    // Worse defense increases offensive success (multiplier > 1.0)
    double pctTDSuccessXP_adj = offenseTeam->getPctTDSuccessXP() * defensiveMultiplier;
    double pctTDMissXP2PtFail_adj = offenseTeam->getPctTDMissXP2PtFail() * defensiveMultiplier;
    double pctTDSuccess2Pt_adj = offenseTeam->getPctTDSuccess2Pt() * defensiveMultiplier;
    double pctFieldGoal_adj = offenseTeam->getPctFieldGoal() * defensiveMultiplier;

    // Negative outcomes (these are base percentages, but the re-normalization will adjust their relative weight)
    double pctTurnover_base = offenseTeam->getPctTurnover();
    double pctOppSafety_base = offenseTeam->getPctOppSafety();
    double pctPunt_base = offenseTeam->getPctPunt();

    // Sum of all adjusted probabilities for normalization
    totalAdjustedProb = pctTDSuccessXP_adj + pctTDMissXP2PtFail_adj + pctTDSuccess2Pt_adj + pctFieldGoal_adj +
        pctTurnover_base + pctOppSafety_base + pctPunt_base;

    // Normalize all probabilities to sum to 1.0
    std::string driveOutcome;
    if (totalAdjustedProb == 0.0) {
        std::cerr << "Warning: Total adjusted probability is zero. Cannot determine drive outcome. Defaulting to Punt." << std::endl;
        driveOutcome = "Punt"; // Fallback to a default negative outcome
    }
    else {
        adjustedProbabilities["TD_SuccessXP"] = pctTDSuccessXP_adj / totalAdjustedProb;
        adjustedProbabilities["TD_MissXP2PtFail"] = pctTDMissXP2PtFail_adj / totalAdjustedProb;
        adjustedProbabilities["TD_Success2Pt"] = pctTDSuccess2Pt_adj / totalAdjustedProb;
        adjustedProbabilities["FieldGoal"] = pctFieldGoal_adj / totalAdjustedProb;
        adjustedProbabilities["Turnover"] = pctTurnover_base / totalAdjustedProb;
        adjustedProbabilities["OpponentSafety"] = pctOppSafety_base / totalAdjustedProb;
        adjustedProbabilities["Punt"] = pctPunt_base / totalAdjustedProb;

        // 5. Randomly select outcome
        std::uniform_real_distribution<double> outcomeDist(0.0, 1.0);
        double randomRoll = outcomeDist(global_rng);
        double cumulativeProb = 0.0;

        for (const auto& pair : adjustedProbabilities) {
            cumulativeProb += pair.second;
            if (randomRoll <= cumulativeProb) {
                driveOutcome = pair.first;
                break;
            }
        }

        // Fallback if no outcome was selected (shouldn't happen with proper normalization)
        if (driveOutcome.empty()) {
            driveOutcome = "Punt";
            std::cerr << "Warning: No drive outcome selected by random roll. Defaulting to Punt." << std::endl;
        }
    }


    // 6. Update scores, possession, and field position based on outcome
    std::string nextPossessionTeamName = "";

    std::cout << "Drive Outcome: " << driveOutcome << std::endl;

    if (driveOutcome == "TD_SuccessXP") {
        if (offenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
            currentGameState.setHomeScore(currentGameState.getHomeScore() + SimulationConstants::TOUCHDOWN_SUCCESSFUL_XP_POINTS);
        }
        else {
            currentGameState.setAwayScore(currentGameState.getAwayScore() + SimulationConstants::TOUCHDOWN_SUCCESSFUL_XP_POINTS);
        }
        std::cout << "  " << offenseTeam->getTeamName() << " scores a TD (7 pts)!" << std::endl;
        if (currentGameState.getCurrentHalfTimeInfo().currentHalf == 2 && updatedHalfTimeInfo.secondsLeftInHalf > 0) {
            std::uniform_real_distribution<double> onsideKickDist(0.0, 1.0);
            if (onsideKickDist(global_rng) <= SimulationConstants::ONSIDE_KICK_SUCCESS_CHANCE) {
                nextPossessionTeamName = offenseTeam->getTeamName(); // Retain possession
                std::cout << "  Successful onside kick! " << offenseTeam->getTeamName() << " retains possession." << std::endl;
            }
            else {
                nextPossessionTeamName = defenseTeam->getTeamName(); // Possession flips
                std::cout << "  Unsuccessful onside kick. Possession flips to " << defenseTeam->getTeamName() << "." << std::endl;
            }
        }
        else {
            nextPossessionTeamName = defenseTeam->getTeamName(); // Possession flips after normal kickoff
        }

    }
    else if (driveOutcome == "TD_MissXP2PtFail") {
        if (offenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
            currentGameState.setHomeScore(currentGameState.getHomeScore() + SimulationConstants::TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS);
        }
        else {
            currentGameState.setAwayScore(currentGameState.getAwayScore() + SimulationConstants::TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS);
        }
        std::cout << "  " << offenseTeam->getTeamName() << " scores a TD (6 pts)!" << std::endl;
        if (currentGameState.getCurrentHalfTimeInfo().currentHalf == 2 && updatedHalfTimeInfo.secondsLeftInHalf > 0) {
            std::uniform_real_distribution<double> onsideKickDist(0.0, 1.0);
            if (onsideKickDist(global_rng) <= SimulationConstants::ONSIDE_KICK_SUCCESS_CHANCE) {
                nextPossessionTeamName = offenseTeam->getTeamName();
                std::cout << "  Successful onside kick! " << offenseTeam->getTeamName() << " retains possession." << std::endl;
            }
            else {
                nextPossessionTeamName = defenseTeam->getTeamName();
                std::cout << "  Unsuccessful onside kick. Possession flips to " << defenseTeam->getTeamName() << "." << std::endl;
            }
        }
        else {
            nextPossessionTeamName = defenseTeam->getTeamName();
        }

    }
    else if (driveOutcome == "TD_Success2Pt") {
        // Corrected: TD (6 pts) + 2pt (2 pts) = 8 points
        if (offenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
            currentGameState.setHomeScore(currentGameState.getHomeScore() + (SimulationConstants::TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS + 2)); // 6 + 2 = 8
        }
        else {
            currentGameState.setAwayScore(currentGameState.getAwayScore() + (SimulationConstants::TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS + 2)); // 6 + 2 = 8
        }
        std::cout << "  " << offenseTeam->getTeamName() << " scores a TD (8 pts)!" << std::endl; // Updated output
        if (currentGameState.getCurrentHalfTimeInfo().currentHalf == 2 && updatedHalfTimeInfo.secondsLeftInHalf > 0) {
            std::uniform_real_distribution<double> onsideKickDist(0.0, 1.0);
            if (onsideKickDist(global_rng) <= SimulationConstants::ONSIDE_KICK_SUCCESS_CHANCE) {
                nextPossessionTeamName = offenseTeam->getTeamName();
                std::cout << "  Successful onside kick! " << offenseTeam->getTeamName() << " retains possession." << std::endl;
            }
            else {
                nextPossessionTeamName = defenseTeam->getTeamName();
                std::cout << "  Unsuccessful onside kick. Possession flips to " << defenseTeam->getTeamName() << "." << std::endl;
            }
        }
        else {
            nextPossessionTeamName = defenseTeam->getTeamName();
        }

    }
    else if (driveOutcome == "FieldGoal") {
        if (offenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
            currentGameState.setHomeScore(currentGameState.getHomeScore() + SimulationConstants::FIELD_GOAL_POINTS);
        }
        else {
            currentGameState.setAwayScore(currentGameState.getAwayScore() + SimulationConstants::FIELD_GOAL_POINTS);
        }
        std::cout << "  " << offenseTeam->getTeamName() << " kicks a Field Goal (3 pts)!" << std::endl;
        nextPossessionTeamName = defenseTeam->getTeamName(); // Possession flips after kickoff

    }
    else if (driveOutcome == "OpponentSafety") {
        if (defenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
            currentGameState.setHomeScore(currentGameState.getHomeScore() + SimulationConstants::SAFETY_POINTS);
        }
        else {
            currentGameState.setAwayScore(currentGameState.getAwayScore() + SimulationConstants::SAFETY_POINTS);
        }
        std::cout << "  Safety! " << defenseTeam->getTeamName() << " gets 2 points!" << std::endl;
        nextPossessionTeamName = defenseTeam->getTeamName(); // Defense gets possession after safety kick

    }
    else if (driveOutcome == "Turnover") {
        std::cout << "  " << offenseTeam->getTeamName() << " commits a Turnover!" << std::endl;
        std::uniform_real_distribution<double> defensiveTDDist(0.0, 1.0);
        if (defensiveTDDist(global_rng) <= SimulationConstants::DEFENSIVE_TD_ON_TURNOVER_CHANCE) {
            // It's a Pick-6 or Fumble-6!
            if (defenseTeam->getTeamName() == currentGameState.getHomeTeam().getTeamName()) {
                currentGameState.setHomeScore(currentGameState.getHomeScore() + SimulationConstants::TOUCHDOWN_SUCCESSFUL_XP_POINTS);
            }
            else {
                currentGameState.setAwayScore(currentGameState.getAwayScore() + SimulationConstants::TOUCHDOWN_SUCCESSFUL_XP_POINTS);
            }
            std::cout << "  Pick-6/Fumble-6! " << defenseTeam->getTeamName() << " scores a TD (7 pts)!" << std::endl;
            nextPossessionTeamName = offenseTeam->getTeamName(); // Possession returns to the team that turned it over
        }
        else {
            // Regular turnover, possession flips
            nextPossessionTeamName = defenseTeam->getTeamName();
            std::cout << "  Possession flips to " << defenseTeam->getTeamName() << "." << std::endl;
        }
    }
    else if (driveOutcome == "Punt") {
        std::cout << "  " << offenseTeam->getTeamName() << " Punts the ball." << std::endl;
        // Check for unsuccessful punt return
        std::uniform_real_distribution<double> puntReturnDist(0.0, 1.0);
        if (puntReturnDist(global_rng) <= SimulationConstants::PUNT_RETURN_UNSUCCESSFUL_CHANCE) {
            nextPossessionTeamName = offenseTeam->getTeamName(); // Retain possession due to failed return
            std::cout << "  Unsuccessful punt return! " << offenseTeam->getTeamName() << " retains possession." << std::endl;
        }
        else {
            nextPossessionTeamName = defenseTeam->getTeamName(); // Possession flips
        }
    }
    else if (driveOutcome == "EndOfHalf") {
        // This outcome should primarily be triggered by simulateGame's clock logic,
        // but if it's explicitly rolled, handle it.
        std::cout << "  Drive ended due to End of Half (natural outcome rolled)." << std::endl;
        // Possession flips for the start of the next half based on initial kickoff
        if (currentGameState.didHomeTeamReceiveKickoff()) {
            nextPossessionTeamName = currentGameState.getAwayTeam().getTeamName();
        }
        else {
            nextPossessionTeamName = currentGameState.getHomeTeam().getTeamName();
        }
        // IMPORTANT: simulateDrive should NOT change currentHalf.
        // It only sets secondsLeftInHalf to 0 and possession.
        updatedHalfTimeInfo.secondsLeftInHalf = 0; // Clock runs out
    }

    // Update the ActiveGame instance with new state
    currentGameState.setCurrentHalfTimeInfo(updatedHalfTimeInfo);
    currentGameState.setPossessionTeamName(nextPossessionTeamName);

    return currentGameState;
}
