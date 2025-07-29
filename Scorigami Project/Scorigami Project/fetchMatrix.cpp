// fetchMatrix.cpp
#include "fetchMatrix.h"
#include "simulateGame.h" // To call the simulateGame function
#include <iostream>
#include <vector>
#include <map>        // For storing score combinations and counts
#include <algorithm>  // For std::sort
#include <iomanip>    // For std::setw

// Struct to hold a score combination, its frequency, and total score for sorting.
struct ScoreCombination {
    int homeScore;
    int awayScore;
    int count;
    int totalScore; // For tie-breaking

    // Custom comparison operator for sorting
    bool operator<(const ScoreCombination& other) const {
        if (count != other.count) {
            return count > other.count; // Sort by frequency (descending)
        }
        return totalScore < other.totalScore; // Then by total score (ascending) for tie-breaking
    }
};

// Function definition for fetchMatrix
std::vector<std::vector<int>> fetchMatrix(ActiveGame initialGameState) { // Changed return type
    // Define matrix dimensions (scores up to 199)
    const int MAX_SCORE = 200;
    std::vector<std::vector<int>> scoreMatrix(MAX_SCORE, std::vector<int>(MAX_SCORE, 0));

    const int NUM_SIMULATIONS = 1000;

    std::cout << "\n--- Running " << NUM_SIMULATIONS << " Game Simulations to Populate Matrix ---" << std::endl;

    for (int i = 0; i < NUM_SIMULATIONS; ++i) {
        // Create a fresh copy of the initial game state for each simulation
        // This ensures each game starts from the same conditions.
        ActiveGame gameToSimulate = initialGameState;

        // Simulate the game
        Score finalScore = simulateGame(gameToSimulate);

        // Directly get home and away scores from the Score object using the new getters
        int homeFinalScore = finalScore.getHomeFinalScore();
        int awayFinalScore = finalScore.getAwayFinalScore();

        // Ensure scores are within matrix bounds
        if (homeFinalScore >= 0 && homeFinalScore < MAX_SCORE &&
            awayFinalScore >= 0 && awayFinalScore < MAX_SCORE) {
            scoreMatrix[homeFinalScore][awayFinalScore]++;
        }
        else {
            std::cerr << "Warning: Final score (" << homeFinalScore << "-" << awayFinalScore
                << ") out of matrix bounds. Skipping this score combination." << std::endl;
        }
    }

    // --- Print the resulting matrix ---
    std::cout << "\n--- Score Matrix (Home vs. Away) ---" << std::endl;
    std::cout << "Rows: Home Team Score, Columns: Away Team Score" << std::endl;

    // Determine max score achieved in simulations to limit print range
    int maxHomeScore = 0;
    int maxAwayScore = 0;
    for (int i = 0; i < MAX_SCORE; ++i) {
        for (int j = 0; j < MAX_SCORE; ++j) {
            if (scoreMatrix[i][j] > 0) {
                if (i > maxHomeScore) maxHomeScore = i;
                if (j > maxAwayScore) maxAwayScore = j;
            }
        }
    }

    // Print column headers (only up to maxAwayScore + a buffer, or first 10)
    std::cout << "      "; // Spacer for row headers
    int printColsUpTo = std::min(MAX_SCORE, std::max(10, maxAwayScore + 5)); // Print at least 10, or up to max + 5
    for (int j = 0; j < printColsUpTo; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << std::endl;

    for (int i = 0; i <= maxHomeScore + 5 && i < MAX_SCORE; ++i) { // Print up to maxHomeScore + a buffer
        std::cout << std::setw(4) << i << " |"; // Row header
        for (int j = 0; j < printColsUpTo; ++j) {
            std::cout << std::setw(4) << scoreMatrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------" << std::endl;


    // --- Find and print the 3 most common score combinations ---
    std::map<std::pair<int, int>, int> scoreCounts;
    for (int i = 0; i < MAX_SCORE; ++i) {
        for (int j = 0; j < MAX_SCORE; ++j) {
            if (scoreMatrix[i][j] > 0) {
                scoreCounts[{i, j}] = scoreMatrix[i][j];
            }
        }
    }

    std::vector<ScoreCombination> commonScores;
    for (const auto& pair : scoreCounts) {
        commonScores.push_back({ pair.first.first, pair.first.second, pair.second, pair.first.first + pair.first.second });
    }

    // Sort the vector using the custom comparison operator
    std::sort(commonScores.begin(), commonScores.end());

    std::cout << "\n--- Top 3 Most Common Score Combinations ---" << std::endl;
    for (int i = 0; i < std::min((int)commonScores.size(), 3); ++i) {
        std::cout << (i + 1) << ". " << commonScores[i].homeScore << " - "
            << commonScores[i].awayScore << " (Occurrences: " << commonScores[i].count << ")" << std::endl;
    }
    std::cout << "---------------------------------------" << std::endl;

    return scoreMatrix; // Return the populated matrix
}
