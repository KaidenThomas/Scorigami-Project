// Team.h
#ifndef TEAM_H
#define TEAM_H

#include <string> // Required for std::string
#include <iostream> // For std::cerr (if warnings are kept)

// The Team class stores various statistical data for a college football team
// across its current and previous seasons.
class Team {
private:
    std::string teamName; // Name of the team

    // Offensive Drive Statistics (in seconds for duration)
    double shortestOffDrive;   // Shortest offensive drive duration (seconds)
    double longestOffDrive;    // Longest offensive drive duration (seconds)
    double avgOffDrive;        // Average offensive drive duration (seconds)
    // Removed avgStartFieldPos and avgEndFieldPos

    // Offensive Drive Outcome Percentages (0.0 to 100.0)
    double pctTurnover;        // Percentage of drives ending in turnover
    double pctOppSafety;       // Percentage of drives ending in opposing safety
    double pctFieldGoal;       // Percentage of drives ending in field goal
    double pctPunt;            // Percentage of drives ending in punt
    double pctTDMissXP2PtFail; // Percentage of drives ending in TD with missed XP/failed 2pt conversion
    double pctTDSuccessXP;     // Percentage of drives ending in TD with successful extra point
    double pctTDSuccess2Pt;    // Percentage of drives ending in TD with successful 2pt conversion
    double pctEndOfHalf;       // Percentage of drives ending in end of half

    // Defensive Rating
    std::string defensiveRating; // 7-tiered defensive rating based on total defense rank

public:
    // Constructor declaration
    Team(const std::string& name,
        double shortestOff, double longestOff, double avgOff,
        double pctTO, double pctOS, double pctFG, double pctP,
        double pctTDMiss, double pctTDXP, double pctTD2Pt, double pctEOH,
        int totalDefenseRank);

    // Getter methods declarations
    std::string getTeamName() const;
    double getShortestOffDrive() const;
    double getLongestOffDrive() const;
    double getAvgOffDrive() const;
    // Removed getAvgStartFieldPos() and getAvgEndFieldPos()
    double getPctTurnover() const;
    double getPctOppSafety() const;
    double getPctFieldGoal() const;
    double getPctPunt() const;
    double getPctTDMissXP2PtFail() const;
    double getPctTDSuccessXP() const;
    double getPctTDSuccess2Pt() const;
    double getPctEndOfHalf() const;
    std::string getDefensiveRating() const;

    // Display method declaration
    void displayTeamStats() const;
};

#endif // TEAM_H