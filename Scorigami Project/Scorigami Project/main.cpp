#include <iostream>
#include <string>
#include <vector>
#include "team.h"
#include "activegame.h"
#include "halftimeinfo.h"
#include "simulatedrive.h"
#include "simulategame.h"
#include "score.h"
#include "fetchmatrix.h"
#include "retrieveData.h"
#include "getTeamStats.h" // Include the new header
#include "getScorigamiHistory.h"

int main() {
    // --- 1. Read team statistics from file ---
    std::string statsFilePath = "Nebraska.txt";
    std::pair<Team, Team> teams = getTeamStats(statsFilePath);
    Team& awayTeam = teams.first;
    Team& homeTeam = teams.second;

    // --- 2. Set up initial game state for simulation ---
    std::cout << "--- Initializing Game State for Simulation ---" << std::endl;

    // Game starts Q4 7:30 (example: adjust as needed)
    std::string currentTimeStr = "15:00";
    int currentQuarter = 1;
    HalfTimeInfo initialHalfTimeInfo = getHalfTimeDetails(currentTimeStr, currentQuarter);

    // Initial scores (set to 0 for both, adjust as needed)
    int initialHomeScore = 0;
    int initialAwayScore = 0;

    // Assume away team has possession at start
    bool homeTeamReceivedKickoff = true;
    std::string initialPossessionTeamName = awayTeam.getTeamName();

    // Create the ActiveGame instance
    ActiveGame currentGameState(
        awayTeam, homeTeam, homeTeamReceivedKickoff,
        initialHomeScore, initialAwayScore,
        initialPossessionTeamName,
        initialHalfTimeInfo
    );

    // --- 3. Call fetchMatrix to run simulations and populate the matrix ---
    const int NUM_SIMULATIONS = 1000;
    std::vector<std::vector<int>> resultMatrix = fetchMatrix(currentGameState);

    // --- 4. Call retrieveData to write results to currentData.txt ---
    retrieveData(resultMatrix, currentGameState, NUM_SIMULATIONS);

    return 0;

}