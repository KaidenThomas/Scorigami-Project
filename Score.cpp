// Score.cpp
#include "Score.h" // Include its own header first
#include <iostream>
#include <algorithm> // Required for std::swap

// Constructor definition
Score::Score(const std::string& winTeam, const std::string& loseTeam,
    int winScore, int loseScore,
    const std::string& home, int homeFScore, int awayFScore, int ot) // Updated constructor signature
    : homeTeam(home),
    homeFinalScore(homeFScore), // Initialize new members
    awayFinalScore(awayFScore), // Initialize new members
    overtimes(ot) {
    // Protocol: Ensure winnerScore is always greater than loserScore.
    // If not, swap the scores and the corresponding team names.
    if (winScore <= loseScore) {
        // If the provided "winner" score is less than or equal to the "loser" score,
        // assume the input was reversed and correct it.
        this->winningTeam = loseTeam;
        this->losingTeam = winTeam;
        this->winnerScore = loseScore;
        this->loserScore = winScore;
        std::cout << "Note: Input scores indicated a reversed outcome. Swapped teams and scores." << std::endl;
    }
    else {
        // If scores are correctly ordered, assign them directly.
        this->winningTeam = winTeam;
        this->losingTeam = loseTeam;
        this->winnerScore = winScore;
        this->loserScore = loseScore;
    }
}

// Getter method definitions
std::string Score::getWinningTeam() const {
    return winningTeam;
}

std::string Score::getLosingTeam() const {
    return losingTeam;
}

int Score::getWinnerScore() const {
    return winnerScore;
}

int Score::getLoserScore() const {
    return loserScore;
}

std::string Score::getHomeTeam() const {
    return homeTeam;
}

int Score::getHomeFinalScore() const { // New getter definition
    return homeFinalScore;
}

int Score::getAwayFinalScore() const { // New getter definition
    return awayFinalScore;
}

int Score::getOvertimes() const {
    return overtimes;
}

// Display method definition
void Score::displayGameSummary() const {
    std::cout << "--- Game Summary ---" << std::endl;
    std::cout << "Winning Team: " << winningTeam << " (" << winnerScore << ")" << std::endl;
    std::cout << "Losing Team: " << losingTeam << " (" << loserScore << ")" << std::endl;
    std::cout << "Home Team Score: " << homeFinalScore << std::endl; // Updated to use new member
    std::cout << "Away Team Score: " << awayFinalScore << std::endl; // Updated to use new member
    std::cout << "Overtimes: " << overtimes << std::endl;
    std::cout << "--------------------" << std::endl;
}
