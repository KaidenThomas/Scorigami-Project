// ActiveGame.h
#ifndef ACTIVEGAME_H
#define ACTIVEGAME_H

#include <string>
#include "Team.h"        // Include Team class declaration
#include "HalfTimeInfo.h" // Include HalfTimeInfo struct declaration

// The ActiveGame class represents a college football game in progress.
class ActiveGame {
private:
    Team homeTeam; // Instance of the home team
    Team awayTeam; // Instance of the away team
    bool homeTeamReceivedKickoff; // True if home team received opening kickoff, false otherwise

    int homeScore; // Current score of the home team
    int awayScore; // Current score of the away team
    std::string possessionTeamName; // Name of the team currently with possession
    // Removed double currentFieldPosition;
    HalfTimeInfo currentHalfTimeInfo; // Current half and time left in half

public:
    // Constructor declaration (now takes 7 arguments)
    ActiveGame(const Team& home, const Team& away, bool homeReceivedKickoff,
        int currentHomeScore, int currentAwayScore,
        const std::string& possessionTeam,
        const HalfTimeInfo& halfTimeInfo);

    // Getter declarations
    const Team& getHomeTeam() const;
    const Team& getAwayTeam() const;
    bool didHomeTeamReceiveKickoff() const;
    int getHomeScore() const;
    int getAwayScore() const;
    std::string getPossessionTeamName() const;
    // Removed double getCurrentFieldPosition() const;
    HalfTimeInfo getCurrentHalfTimeInfo() const;

    // Setter declarations for updating game state
    void setHomeScore(int score);
    void setAwayScore(int score);
    void setPossessionTeamName(const std::string& name);
    // Removed void setCurrentFieldPosition(double pos);
    void setCurrentHalfTimeInfo(const HalfTimeInfo& info);

    // Display method declaration
    void displayCurrentGameStatus() const;
};

#endif // ACTIVEGAME_H