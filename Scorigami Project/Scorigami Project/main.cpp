#include <iostream>
#include <string>
#include <vector> // required for std::vector
#include "team.h"
#include "activegame.h"
#include "halftimeinfo.h"
#include "simulatedrive.h"
#include "simulategame.h"
#include "score.h"
#include "fetchmatrix.h"
#include "retrieveData.h" // Include the new retrieveData header

int main() {
    // --- 1. define team statistics (distinct for ohio state and notre dame) ---

    // Ohio State (OSU) - top-tier team metrics
    double osuShortestOff = 40.0;   // explosive, quick drives
    double osuLongestOff = 220.0;   // ~3.6 minutes
    double osuAvgOff = 140.0;       // ~2.3 minutes
    double osuPctTO = 7.0;          // very low turnover rate
    double osuPctOS = 0.3;          // extremely low opponent safety chance
    double osuPctFG = 10.0;         // lower field goal rate (prefer TDs)
    double osuPctP = 20.0;          // lower punt rate
    double osuPctTDMiss = 3.0;      // very low missed XP/failed 2pt TD rate
    double osuPctTDXP = 50.0;       // high successful XP TD rate
    double osuPctTD2Pt = 9.7;       // higher successful 2pt TD rate (total 100%)
    double osuPctEOH = 0.0;
    int osuDefenseRank = 1;         // "Juggernaut" defense

    // Notre Dame (ND) - high-tier independent team metrics
    double ndShortestOff = 55.0;    // solid, efficient drives
    double ndLongestOff = 280.0;    // ~4.6 minutes
    double ndAvgOff = 170.0;        // ~2.8 minutes
    double ndPctTO = 10.0;          // moderate turnover rate
    double ndPctOS = 0.5;           // low opponent safety chance
    double ndPctFG = 15.0;          // moderate field goal rate
    double ndPctP = 25.0;           // moderate punt rate
    double ndPctTDMiss = 5.0;       // moderate missed XP/failed 2pt TD rate
    double ndPctTDXP = 40.0;        // good successful XP TD rate
    double ndPctTD2Pt = 4.5;        // moderate successful 2pt TD rate (total 100%)
    double ndPctEOH = 0.0;
    int ndDefenseRank = 10;         // "Terrifying" defense

    // Create team objects with their specific metrics
    Team ohioState("Ohio State",
        osuShortestOff, osuLongestOff, osuAvgOff,
        osuPctTO, osuPctOS, osuPctFG, osuPctP,
        osuPctTDMiss, osuPctTDXP, osuPctTD2Pt, osuPctEOH,
        osuDefenseRank);

    Team notreDame("Notre Dame",
        ndShortestOff, ndLongestOff, ndAvgOff,
        ndPctTO, ndPctOS, ndPctFG, ndPctP,
        ndPctTDMiss, ndPctTDXP, ndPctTD2Pt, ndPctEOH,
        ndDefenseRank);

    // --- 2. Set up initial game state for national championship (OSU vs ND, 2025) ---
    std::cout << "--- Initializing Game State for National Championship Simulation ---" << std::endl;

    // Game starts Q4 7:30
    std::string currentTimeStr = "07:30";
    int currentQuarter = 4; // Fourth quarter
    HalfTimeInfo initialHalfTimeInfo = getHalfTimeDetails(currentTimeStr, currentQuarter);

    // Initial scores: OSU 31, ND 15
    int initialHomeScore = 31; // Ohio State's score
    int initialAwayScore = 15; // Notre Dame's score

    // Assuming Ohio State (home team) and Notre Dame (away team).
    // For a 4th quarter start, assume the team that is currently trailing gets possession,
    // or if it's a fresh quarter start, it depends on who received the opening kickoff.
    // Let's assume Notre Dame (away) has possession as they are trailing.
    bool homeTeamReceivedKickoff = true; // This affects 3rd quarter kickoff, but not directly 4th quarter start
    std::string initialPossessionTeamName = notreDame.getTeamName();

    // Create the ActiveGame instance
    ActiveGame currentGameState(ohioState, notreDame, homeTeamReceivedKickoff,
        initialHomeScore, initialAwayScore,
        initialPossessionTeamName,
        initialHalfTimeInfo);

    // --- 3. Call fetchMatrix to run simulations and populate the matrix ---
    // The returned matrix is now stored in 'resultMatrix'
    // Note: NUM_SIMULATIONS is hardcoded in fetchMatrix.cpp.
    // For consistency, we'll define it here for passing to retrieveData.
    const int NUM_SIMULATIONS = 1000; // Define here for passing to retrieveData

    std::vector<std::vector<int>> resultMatrix = fetchMatrix(currentGameState);

    // --- 4. Call retrieveData to write results to currentData.txt ---
    // This function will take the score matrix, the current game state,
    // and the total number of simulations, and write the required information to the text file.
    retrieveData(resultMatrix, currentGameState, NUM_SIMULATIONS);

    return 0;
}
