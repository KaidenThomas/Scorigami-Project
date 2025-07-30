// retrieveData.cpp
#include "retrieveData.h" // Include its own header first
#include <fstream>    // Required for file output (std::ofstream)
#include <iostream>   // Required for console output (std::cerr, std::cout)
#include <iomanip>    // Required for std::setw, std::setfill, std::fixed, std::setprecision for time and percentage formatting
#include <algorithm>  // Required for std::min, std::sort
#include <sstream>    // Required for std::stringstream to format time string

// Struct to hold a score combination, its frequency, and total score for sorting.
// This is a local definition, similar to the one in fetchMatrix.cpp,
// to handle the sorting of score combinations for the top 3 display.
struct ScoreCombination {
    int homeScore;
    int awayScore;
    int count;
    int totalScore; // For tie-breaking

    // Custom comparison operator for sorting
    // Sorts by frequency (descending) first, then by total score (ascending) for ties.
    bool operator<(const ScoreCombination& other) const {
        if (count != other.count) {
            return count > other.count; // Sort by frequency (descending)
        }
        return totalScore < other.totalScore; // Then by total score (ascending) for tie-breaking
    }
}; // Missing semicolon was a common cause for '{' errors in structs

// Function definition for retrieveData
void retrieveData(const std::vector<std::vector<int>>& scoreMatrix, const ActiveGame& currentGameState, int numSimulations) {
    // Open the output file. std::ios::trunc ensures the file is
    // truncated (emptied) if it exists, or created if it doesn't.
    std::ofstream outputFile("currentData.txt", std::ios::trunc);

    // Check if the file was opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open currentData.txt for writing." << std::endl;
        return; // Exit the function if file cannot be opened
    }

    // --- 1. Extract Current Game State Information ---
    // Get time information from ActiveGame
    HalfTimeInfo halfTimeInfo = currentGameState.getCurrentHalfTimeInfo();
    int secondsLeftInHalf = halfTimeInfo.secondsLeftInHalf;
    int currentHalf = halfTimeInfo.currentHalf;

    int currentQuarter;
    int secondsLeftInCurrentQuarterDisplay;

    // Logic to determine current quarter and display time, adapted from simulateGame.cpp
    if (currentHalf == 1) {
        if (secondsLeftInHalf > (15 * 60)) { // More than 15 minutes left in the half means it's Q1
            currentQuarter = 1;
            secondsLeftInCurrentQuarterDisplay = secondsLeftInHalf - (15 * 60);
        }
        else { // 15 minutes or less left in the half means it's Q2
            currentQuarter = 2;
            secondsLeftInCurrentQuarterDisplay = secondsLeftInHalf;
        }
    }
    else { // currentHalf == 2 (second half)
        if (secondsLeftInHalf > (15 * 60)) { // More than 15 minutes left means it's Q3
            currentQuarter = 3;
            secondsLeftInCurrentQuarterDisplay = secondsLeftInHalf - (15 * 60);
        }
        else { // 15 minutes or less left means it's Q4
            currentQuarter = 4;
            secondsLeftInCurrentQuarterDisplay = secondsLeftInHalf;
        }
    }
    // Ensure display time doesn't go negative if a drive overran the quarter.
    secondsLeftInCurrentQuarterDisplay = std::max(0, secondsLeftInCurrentQuarterDisplay);

    // Format minutes and seconds for display (MM:SS)
    int displayMinutes = secondsLeftInCurrentQuarterDisplay / 60;
    int displaySeconds = secondsLeftInCurrentQuarterDisplay % 60;

    // Use std::setfill and std::setw for consistent two-digit seconds
    std::stringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << displayMinutes << ":"
        << std::setfill('0') << std::setw(2) << displaySeconds;
    std::string currentTimeStr = timeStream.str();

    // Get team names and scores
    std::string homeTeamName = currentGameState.getHomeTeam().getTeamName();
    std::string awayTeamName = currentGameState.getAwayTeam().getTeamName();
    int homeScore = currentGameState.getHomeScore();
    int awayScore = currentGameState.getAwayScore();
    std::string possessionTeamName = currentGameState.getPossessionTeamName(); // Get possession team name

    // --- 2. Calculate Top 3 Most Likely Final Score Combinations ---
    // This logic is adapted from fetchMatrix.cpp
    const int MAX_SCORE = 200; // Assuming the same MAX_SCORE as in fetchMatrix.cpp
    std::map<std::pair<int, int>, int> scoreCounts;

    // Populate scoreCounts map from the input scoreMatrix
    for (int i = 0; i < MAX_SCORE; ++i) {
        for (int j = 0; j < MAX_SCORE; ++j) {
            if (scoreMatrix[i][j] > 0) {
                scoreCounts[{i, j}] = scoreMatrix[i][j];
            }
        }
    }

    // Transfer map entries to a vector of ScoreCombination structs
    std::vector<ScoreCombination> commonScores;
    for (const auto& pair : scoreCounts) {
        commonScores.push_back({ pair.first.first, pair.first.second, pair.second, pair.first.first + pair.first.second });
    }

    // Sort the vector to get the most common scores
    std::sort(commonScores.begin(), commonScores.end());

    // --- 3. Write Information to currentData.txt ---
    outputFile << "CurrentTime: " << currentTimeStr << std::endl;
    outputFile << "CurrentQuarter: " << currentQuarter << std::endl;
    outputFile << "HomeTeam: " << homeTeamName << std::endl;
    outputFile << "AwayTeam: " << awayTeamName << std::endl;
    outputFile << "HomeScore: " << homeScore << std::endl;
    outputFile << "AwayScore: " << awayScore << std::endl;
    outputFile << "PossessionTeam: " << possessionTeamName << std::endl; // Added possession team

    outputFile << "--- Top 3 Most Likely Final Score Combinations ---" << std::endl;
    for (int i = 0; i < std::min((int)commonScores.size(), 3); ++i) {
        double percentage = (static_cast<double>(commonScores[i].count) / numSimulations) * 100.0;
        outputFile << "TopScore" << (i + 1) << ": "
            << commonScores[i].homeScore << "-"
            << commonScores[i].awayScore << " (Chance: ";

        // Implement the edge case for displaying 0.0% as <0.1%
        if (percentage == 0.0) {
            outputFile << "<0.1%";
        }
        else {
            outputFile << std::fixed << std::setprecision(1) << percentage << "%)";
        }
        outputFile << std::endl;
    }

    // Placeholder for Scorigami percentage (to be implemented later)
    outputFile << "ScorigamiChance: N/A" << std::endl;

    // Close the file
    outputFile.close();

    std::cout << "Data successfully written to currentData.txt" << std::endl;
}
