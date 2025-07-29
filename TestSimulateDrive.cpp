#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <map>
#include <algorithm> // For std::min, std::max

// Include all necessary .cpp files as a single compilation unit for demonstration.
// In a real project, you would include their corresponding .h files.
#include "Score.cpp"
#include "Team.cpp"
#include "HalfTimeInfo.cpp" // Contains HalfTimeInfo struct and getHalfTimeDetails function
#include "activeGame.cpp"
#include "simulateDrive.cpp" // Contains the simulateDrive function

// Note: The global_rng static variable is defined within simulateDrive.cpp.
// If this main.cpp is compiled as a separate compilation unit and linked,
// ensure global_rng is properly declared as 'extern' in a header or
// its definition is only in one .cpp file. For this self-contained example,
// including simulateDrive.cpp directly handles it.

int main() {
    // 1. Define Team objects for Ohio State and Michigan
    // Using plausible placeholder stats for a top-tier matchup
    Team ohioState("Ohio State",
        10.0, 240.0, 120.0, 35.0, 65.0, // Shortest, Longest, Avg Drive Duration (seconds), Avg Start/End Field Pos
        8.0, 0.1, 18.0, 25.0,          // Turnover, Opponent Safety, Field Goal, Punt %
        2.0, 40.0, 6.9, 0.0,           // TD Miss XP/2Pt Fail, TD Success XP, TD Success 2Pt, End Of Half %
        5);                            // Total Defense Rank (Terrifying)

    Team michigan("Michigan",
        12.0, 220.0, 110.0, 30.0, 60.0,
        9.0, 0.2, 20.0, 28.0,
        1.5, 35.0, 6.3, 0.0,
        1);                             // Total Defense Rank (Juggernaut)

    // Display initial team stats for context
    std::cout << "--- Initial Team Statistics ---" << std::endl;
    ohioState.displayTeamStats();
    michigan.displayTeamStats();

    // 2. Determine initial game state for the current drive
    // Starting at 7:30 left in the first quarter (Q1)
    std::string initialTimeStr = "07:30";
    int initialQuarter = 1;
    HalfTimeInfo initialHalfTimeInfo = getHalfTimeDetails(initialTimeStr, initialQuarter);

    // Assume Ohio State is the home team and receives the opening kickoff
    bool homeTeamReceivedKickoff = true; // Ohio State receives kickoff
    int initialHomeScore = 0;
    int initialAwayScore = 0;
    std::string initialPossessionTeam = ohioState.getTeamName(); // Ohio State has possession
    double initialFieldPosition = 25.0; // Standard kickoff return to 25-yard line

    // 3. Create the initial ActiveGame instance for the current drive
    ActiveGame currentDriveState(ohioState, michigan, homeTeamReceivedKickoff,
        initialHomeScore, initialAwayScore,
        initialPossessionTeam, initialFieldPosition,
        initialHalfTimeInfo);

    std::cout << "\n--- Game State BEFORE Simulating Current Drive ---" << std::endl;
    currentDriveState.displayCurrentGameStatus();

    // 4. Simulate the current drive
    std::cout << "\n--- Simulating ONE Drive ---" << std::endl;
    ActiveGame nextDriveState = simulateDrive(currentDriveState);

    // 5. Display the game state for the beginning of the next drive
    std::cout << "\n--- Game State AFTER Simulating Drive (Start of Next Drive) ---" << std::endl;
    nextDriveState.displayCurrentGameStatus();

    return 0;
}