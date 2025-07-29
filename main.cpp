#include <iostream>
#include <string>
#include <vector> // Required for std::vector
#include "Team.h"
#include "ActiveGame.h"
#include "HalfTimeInfo.h"
#include "simulateDrive.h"
#include "simulateGame.h"
#include "Score.h"
#include "fetchMatrix.h" // Include fetchMatrix header

int main() {
    // --- 1. Define Team Statistics (Distinct for Ohio State and Notre Dame) ---

    // Ohio State (OSU) - Top-tier team metrics
    double osuShortestOff = 40.0;   // Explosive, quick drives
    double osuLongestOff = 220.0;   // ~3.6 minutes
    double osuAvgOff = 140.0;       // ~2.3 minutes
    double osuPctTO = 7.0;          // Very low turnover rate
    double osuPctOS = 0.3;          // Extremely low opponent safety chance
    double osuPctFG = 10.0;         // Lower field goal rate (prefer TDs)
    double osuPctP = 20.0;          // Lower punt rate
    double osuPctTDMiss = 3.0;      // Very low missed XP/failed 2pt TD rate
    double osuPctTDXP = 50.0;       // High successful XP TD rate
    double osuPctTD2Pt = 9.7;       // Higher successful 2pt TD rate (total 100%)
    double osuPctEOH = 0.0;
    int osuDefenseRank = 1;         // "Juggernaut" defense

    // Notre Dame (ND) - High-tier independent team metrics
    double ndShortestOff = 55.0;    // Solid, efficient drives
    double ndLongestOff = 280.0;    // ~4.6 minutes
    double ndAvgOff = 170.0;        // ~2.8 minutes
    double ndPctTO = 10.0;          // Moderate turnover rate
    double ndPctOS = 0.5;           // Low opponent safety chance
    double ndPctFG = 15.0;          // Moderate field goal rate
    double ndPctP = 25.0;           // Moderate punt rate
    double ndPctTDMiss = 5.0;       // Moderate missed XP/failed 2pt TD rate
    double ndPctTDXP = 40.0;        // Good successful XP TD rate
    double ndPctTD2Pt = 4.5;        // Moderate successful 2pt TD rate (total 100%)
    double ndPctEOH = 0.0;
    int ndDefenseRank = 10;         // "Terrifying" defense

    // Create Team objects with their specific metrics
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

    // --- 2. Set up Initial Game State for National Championship (OSU vs ND, 2025) ---
    std::cout << "--- Initializing Game State for National Championship Simulation ---" << std::endl;

    // Game starts Q4 7:30
    std::string currentTimeStr = "07:30";
    int currentQuarter = 4; // Fourth Quarter
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
    std::vector<std::vector<int>> resultMatrix = fetchMatrix(currentGameState);

    // You can now use 'resultMatrix' for further analysis if needed.
    // For example, you could pass it to another function or process it here.

    return 0;
}