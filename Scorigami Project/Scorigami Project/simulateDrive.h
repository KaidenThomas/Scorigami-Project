// simulateDrive.h
#ifndef SIMULATEDRIVE_H
#define SIMULATEDRIVE_H

#include <string>
#include <map>
#include "ActiveGame.h" // Include ActiveGame class declaration

// --- SIMULATION CONSTANTS AND FORMULAS DECLARATIONS ---
// These are declared here and defined in simulateDrive.cpp
namespace SimulationConstants {
    extern const std::map<std::string, double> DEFENSIVE_MULTIPLIERS;

    extern const double ONSIDE_KICK_SUCCESS_CHANCE;
    extern const double PUNT_RETURN_UNSUCCESSFUL_CHANCE;
    extern const double DEFENSIVE_TD_ON_TURNOVER_CHANCE; // Chance of defensive TD on turnover

    extern const int TOUCHDOWN_SUCCESSFUL_XP_POINTS;
    extern const int TOUCHDOWN_MISSED_XP_FAILED_2PT_POINTS;
    extern const int FIELD_GOAL_POINTS;
    extern const int SAFETY_POINTS;

    extern const double DRIVE_DURATION_STD_DEV_FACTOR;
    // Removed FIELD_POSITION_STD_DEV_FACTOR, FIELD_POSITION_MIN, FIELD_POSITION_MAX
}
// --- END SIMULATION CONSTANTS AND FORMULAS DECLARATIONS ---

// Function declaration for simulateDrive
ActiveGame simulateDrive(ActiveGame currentGameState);

#endif // SIMULATEDRIVE_H
