// ActiveGame.cpp
#include "ActiveGame.h" // Include its own header first
#include <iostream>

// Constructor definition
ActiveGame::ActiveGame(const Team& home, const Team& away, bool homeReceivedKickoff,
    int currentHomeScore, int currentAwayScore,
    const std::string& possessionTeam,
    const HalfTimeInfo& halfTimeInfo)
    : homeTeam(home),
    awayTeam(away),
    homeTeamReceivedKickoff(homeReceivedKickoff),
    homeScore(currentHomeScore),
    awayScore(currentAwayScore),
    possessionTeamName(possessionTeam),
    currentHalfTimeInfo(halfTimeInfo) {}

// Getter definitions
const Team& ActiveGame::getHomeTeam() const {
    return homeTeam;
}

const Team& ActiveGame::getAwayTeam() const {
    return awayTeam;
}

bool ActiveGame::didHomeTeamReceiveKickoff() const {
    return homeTeamReceivedKickoff;
}

int ActiveGame::getHomeScore() const {
    return homeScore;
}

int ActiveGame::getAwayScore() const {
    return awayScore;
}

std::string ActiveGame::getPossessionTeamName() const {
    return possessionTeamName;
}

// Removed double ActiveGame::getCurrentFieldPosition() const { return currentFieldPosition; }

HalfTimeInfo ActiveGame::getCurrentHalfTimeInfo() const {
    return currentHalfTimeInfo;
}

// Setter definitions
void ActiveGame::setHomeScore(int score) { homeScore = score; }
void ActiveGame::setAwayScore(int score) { awayScore = score; }
void ActiveGame::setPossessionTeamName(const std::string& name) { possessionTeamName = name; }
// Removed void ActiveGame::setCurrentFieldPosition(double pos) { currentFieldPosition = pos; }
void ActiveGame::setCurrentHalfTimeInfo(const HalfTimeInfo& info) { currentHalfTimeInfo = info; }

// Display method definition
void ActiveGame::displayCurrentGameStatus() const {
    std::cout << "--- Active Game Status ---" << std::endl;
    std::cout << "Home Team: " << homeTeam.getTeamName() << " (" << homeScore << ")" << std::endl;
    std::cout << "Away Team: " << awayTeam.getTeamName() << " (" << awayScore << ")" << std::endl;
    std::cout << "Opening Kickoff Received by: "
        << (homeTeamReceivedKickoff ? homeTeam.getTeamName() : awayTeam.getTeamName())
        << std::endl;
    std::cout << "Possession: " << possessionTeamName << std::endl;
    // Removed "Field Position: " << currentFieldPosition << " yards from goal line"
    std::cout << "Current Half: " << currentHalfTimeInfo.currentHalf << std::endl;
    std::cout << "Seconds Left in Half: " << currentHalfTimeInfo.secondsLeftInHalf << std::endl;
    std::cout << "-------------------------" << std::endl;
}
