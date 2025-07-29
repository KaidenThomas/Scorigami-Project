#include <string>
#include <iostream>
#include <algorithm> // For std::max
#include "simulateGame.h"
#include "simulateDrive.h" // Include simulateDrive for drive-by-drive simulation
#include "Score.h" // Ensure Score.h is included for the Score class definition

// Function to simulate an entire college football game
Score simulateGame(ActiveGame initialGameState) {
    ActiveGame currentGameState = initialGameState;
    int overtimes = 0;
    bool gameOver = false;

    std::cout << "\n--- Starting Game Simulation ---" << std::endl;
    currentGameState.displayCurrentGameStatus();

    while (!gameOver) {
        // --- Dynamic Quarter Determination for Display and Logic ---
        // This logic determines the current quarter based on the time left in the half.
        // It's updated at the beginning of each loop iteration.
        int currentQuarter;
        if (currentGameState.getCurrentHalfTimeInfo().currentHalf == 1) {
            // If secondsLeftInHalf is > 900 (15 minutes), it's Q1. Otherwise, Q2.
            if (currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf > (15 * 60)) {
                currentQuarter = 1;
            }
            else {
                currentQuarter = 2;
            }
        }
        else { // currentHalf == 2
            // If secondsLeftInHalf is > 900 (from start of 3rd quarter), it implies it's Q3. Otherwise, Q4.
            if (currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf > (15 * 60)) {
                currentQuarter = 3;
            }
            else {
                currentQuarter = 4;
            }
        }

        // Calculate time left in the *current quarter* for display purposes.
        // This is derived from secondsLeftInHalf, adjusted for the quarter.
        int secondsLeftInCurrentQuarterDisplay;
        if (currentQuarter == 1 || currentQuarter == 3) {
            // For Q1 and Q3, secondsLeftInHalf includes the time for the *next* quarter.
            // So, subtract 15 minutes (900 seconds) to get time left in current quarter.
            secondsLeftInCurrentQuarterDisplay = currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf - (15 * 60);
        }
        else {
            // For Q2 and Q4, secondsLeftInHalf directly represents time left in this quarter.
            secondsLeftInCurrentQuarterDisplay = currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf;
        }
        // Ensure display time doesn't go negative if a drive overran the quarter.
        secondsLeftInCurrentQuarterDisplay = std::max(0, secondsLeftInCurrentQuarterDisplay);


        // --- Check for Half-End / Game-End Conditions (only at natural drive breaks) ---
        // This block executes if the previous drive (or initial state) caused the clock to run out
        // for the *current half*.
        if (currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf <= 0) {
            // Use the 'currentQuarter' derived at the top of the loop for robust transition logic.
            // This ensures we correctly identify if it's the end of Q2 (halftime) or Q4 (regulation end).
            if (currentQuarter == 2) { // End of 1st half (Q2 clock ran out)
                std::cout << "\n--- HALFTIME ---" << std::endl;
                // Transition to 2nd half (Q3). Clock for 2nd half starts at 30:00 (15 mins for Q3 + 15 mins for Q4)
                HalfTimeInfo nextHalfInfo = { 15 * 60 + 15 * 60, 2 }; // 1800 seconds total for 2nd half
                currentGameState.setCurrentHalfTimeInfo(nextHalfInfo);
                // Possession flips for 3rd quarter kickoff.
                if (currentGameState.didHomeTeamReceiveKickoff()) {
                    currentGameState.setPossessionTeamName(currentGameState.getAwayTeam().getTeamName());
                }
                else {
                    currentGameState.setPossessionTeamName(currentGameState.getHomeTeam().getTeamName());
                }
                // Removed currentGameState.setCurrentFieldPosition(25.0);
                currentGameState.displayCurrentGameStatus();
                continue; // Skip to the next iteration to start the new half properly
            }
            else if (currentQuarter == 4) { // End of 2nd half (Q4 clock ran out)
                if (currentGameState.getHomeScore() == currentGameState.getAwayScore()) {
                    // Game is tied, enter overtime
                    overtimes++;
                    std::cout << "\n--- Entering Overtime " << overtimes << " ---" << std::endl;
                    // Reset clock for OT drive (each team gets a possession from 25-yard line).
                    // We'll use 15 minutes as a conceptual "quarter" for OT.
                    HalfTimeInfo otInfo = { 15 * 60, 2 }; // Still Half 2 context for OT
                    currentGameState.setCurrentHalfTimeInfo(otInfo);

                    // Alternate possession for overtime.
                    std::string homeTeamName = currentGameState.getHomeTeam().getTeamName();
                    std::string awayTeamName = currentGameState.getAwayTeam().getTeamName();
                    if (currentGameState.getPossessionTeamName() == homeTeamName) {
                        currentGameState.setPossessionTeamName(awayTeamName);
                    }
                    else {
                        currentGameState.setPossessionTeamName(homeTeamName);
                    }
                    // Removed currentGameState.setCurrentFieldPosition(25.0);
                    currentGameState.displayCurrentGameStatus();
                    continue; // Skip to next iteration to start OT drive
                }
                else {
                    // Game ended in regulation, one team leads.
                    gameOver = true;
                    break; // Exit the game loop
                }
            }
        }

        // --- Simulate a Single Drive ---
        std::cout << "\n--- Simulating Drive for " << currentGameState.getPossessionTeamName()
            << " (Q" << currentQuarter << ", "
            << secondsLeftInCurrentQuarterDisplay / 60 << ":"
            << (secondsLeftInCurrentQuarterDisplay % 60 < 10 ? "0" : "") // For leading zero
            << secondsLeftInCurrentQuarterDisplay % 60
            << " left) ---" << std::endl;
        currentGameState.displayCurrentGameStatus(); // Display state before the drive
        currentGameState = simulateDrive(currentGameState);
        std::cout << "Drive completed. Current status:" << std::endl;
        currentGameState.displayCurrentGameStatus(); // Display state after the drive

        // --- Check for Game Over Conditions After a Drive ---
        // If in overtime and a score has created a lead, the game is over.
        if (overtimes > 0 && currentGameState.getHomeScore() != currentGameState.getAwayScore()) {
            gameOver = true;
        }
        // If the clock ran out in the 4th quarter (currentHalf == 2, secondsLeftInHalf <= 0)
        // AND the scores are NOT tied, the game is over.
        // This is a redundant check for regulation end, but good for clarity.
        if (currentQuarter == 4 &&
            currentGameState.getCurrentHalfTimeInfo().secondsLeftInHalf <= 0 &&
            currentGameState.getHomeScore() != currentGameState.getAwayScore()) {
            gameOver = true;
        }
    }

    // --- Determine Final Score and Return Score Object ---
    std::string winningTeamName;
    std::string losingTeamName;
    int winnerScore;
    int loserScore;
    std::string homeTeamName = initialGameState.getHomeTeam().getTeamName();
    int finalHomeScore = currentGameState.getHomeScore(); // Get final home score
    int finalAwayScore = currentGameState.getAwayScore(); // Get final away score


    if (currentGameState.getHomeScore() > currentGameState.getAwayScore()) {
        winningTeamName = currentGameState.getHomeTeam().getTeamName();
        losingTeamName = currentGameState.getAwayTeam().getTeamName();
        winnerScore = currentGameState.getHomeScore();
        loserScore = currentGameState.getAwayScore();
    }
    else if (currentGameState.getAwayScore() > currentGameState.getHomeScore()) {
        winningTeamName = currentGameState.getAwayTeam().getTeamName();
        losingTeamName = currentGameState.getHomeTeam().getTeamName();
        winnerScore = currentGameState.getAwayScore();
        loserScore = currentGameState.getHomeScore();
    }
    else {
        // This case should ideally not be reached in college football due to overtime rules,
        // as OT continues until a winner is decided. This is a fallback.
        winningTeamName = "Tied Game";
        losingTeamName = "Tied Game";
        winnerScore = currentGameState.getHomeScore();
        loserScore = currentGameState.getAwayScore();
        std::cout << "Warning: Game ended in a tie. Overtime rules might need further refinement for continuous play." << std::endl;
    }

    // Pass final home and away scores to the Score constructor
    Score finalScore(winningTeamName, losingTeamName, winnerScore, loserScore,
        homeTeamName, finalHomeScore, finalAwayScore, overtimes);
    finalScore.displayGameSummary();

    return finalScore;
}
