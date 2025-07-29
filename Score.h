// Score.h
#ifndef SCORE_H
#define SCORE_H

#include <string> // Required for std::string

// The Score class stores information about the final score of a college football game.
class Score {
private:
    std::string winningTeam;    // Name of the winning team
    std::string losingTeam;     // Name of the losing team
    int winnerScore;            // Score of the winning team
    int loserScore;             // Score of the losing team
    std::string homeTeam;       // Name of the home team (original home team)
    int homeFinalScore;         // Final score of the home team (new)
    int awayFinalScore;         // Final score of the away team (new)
    int overtimes;              // Number of overtimes played

public:
    // Constructor declaration
    Score(const std::string& winTeam, const std::string& loseTeam,
        int winScore, int loseScore,
        const std::string& home, int homeFScore, int awayFScore, int ot); // Updated constructor signature

    // Getter methods declarations
    std::string getWinningTeam() const;
    std::string getLosingTeam() const;
    int getWinnerScore() const;
    int getLoserScore() const;
    std::string getHomeTeam() const;
    int getHomeFinalScore() const; // New getter
    int getAwayFinalScore() const; // New getter
    int getOvertimes() const;

    // Optional: A method to display the game summary
    void displayGameSummary() const;
};

#endif // SCORE_H
