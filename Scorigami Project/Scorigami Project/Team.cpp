// Team.cpp
#include "Team.h" // Include its own header first

// Constructor definition
Team::Team(const std::string& name,
    double shortestOff, double longestOff, double avgOff,
    double pctTO, double pctOS, double pctFG, double pctP,
    double pctTDMiss, double pctTDXP, double pctTD2Pt, double pctEOH,
    int totalDefenseRank)
    : teamName(name),
    shortestOffDrive(shortestOff),
    longestOffDrive(longestOff),
    avgOffDrive(avgOff),
    pctTurnover(pctTO),
    pctOppSafety(pctOS),
    pctFieldGoal(pctFG),
    pctPunt(pctP),
    pctTDMissXP2PtFail(pctTDMiss),
    pctTDSuccessXP(pctTDXP),
    pctTDSuccess2Pt(pctTD2Pt),
    pctEndOfHalf(pctEOH) {
    // Determine defensive rating based on totalDefenseRank (out of 133 FBS teams)
    if (totalDefenseRank >= 1 && totalDefenseRank <= 2) { // Top 2% (133 * 0.02 = 2.66)
        defensiveRating = "Juggernaut";
    }
    else if (totalDefenseRank > 2 && totalDefenseRank <= 9) { // Next 5% (133 * 0.05 = 6.65, 2+7=9)
        defensiveRating = "Terrifying";
    }
    else if (totalDefenseRank > 9 && totalDefenseRank <= 22) { // Next 10% (133 * 0.10 = 13.3, 9+13=22)
        defensiveRating = "Scary";
    }
    else if (totalDefenseRank > 22 && totalDefenseRank <= 49) { // Next 20% (133 * 0.20 = 26.6, 22+27=49)
        defensiveRating = "Passing";
    }
    else if (totalDefenseRank > 49 && totalDefenseRank <= 86) { // Next 28% (133 * 0.28 = 37.24, 49+37=86)
        defensiveRating = "Average";
    }
    else if (totalDefenseRank > 86 && totalDefenseRank <= 113) { // Next 20% (133 * 0.20 = 26.6, 86+27=113)
        defensiveRating = "Bad";
    }
    else if (totalDefenseRank > 113 && totalDefenseRank <= 133) { // Last 15% (133 * 0.15 = 19.95, 113+20=133)
        defensiveRating = "Terrible";
    }
    else {
        defensiveRating = "N/A (Invalid Rank)"; // Handle ranks outside 1-133
        std::cerr << "Warning: Invalid totalDefenseRank (" << totalDefenseRank << ") provided for " << name << ". Defensive rating set to N/A." << std::endl;
    }
}

// Getter methods definitions
std::string Team::getTeamName() const { return teamName; }
double Team::getShortestOffDrive() const { return shortestOffDrive; }
double Team::getLongestOffDrive() const { return longestOffDrive; }
double Team::getAvgOffDrive() const { return avgOffDrive; }
double Team::getPctTurnover() const { return pctTurnover; }
double Team::getPctOppSafety() const { return pctOppSafety; }
double Team::getPctFieldGoal() const { return pctFieldGoal; }
double Team::getPctPunt() const { return pctPunt; }
double Team::getPctTDMissXP2PtFail() const { return pctTDMissXP2PtFail; }
double Team::getPctTDSuccessXP() const { return pctTDSuccessXP; }
double Team::getPctTDSuccess2Pt() const { return pctTDSuccess2Pt; }
double Team::getPctEndOfHalf() const { return pctEndOfHalf; }
std::string Team::getDefensiveRating() const { return defensiveRating; }

// Display method definition
void Team::displayTeamStats() const {
    std::cout << "--- Team Statistics for " << teamName << " ---" << std::endl;
    std::cout << "Offensive Drives:" << std::endl;
    std::cout << "  Shortest Duration: " << shortestOffDrive << " seconds" << std::endl;
    std::cout << "  Longest Duration: " << longestOffDrive << " seconds" << std::endl;
    std::cout << "  Average Duration: " << avgOffDrive << " seconds" << std::endl;
    std::cout << "Drive Outcomes (%):" << std::endl;
    std::cout << "  Turnover: " << pctTurnover << "%" << std::endl;
    std::cout << "  Opponent Safety: " << pctOppSafety << "%" << std::endl;
    std::cout << "  Field Goal: " << pctFieldGoal << "%" << std::endl;
    std::cout << "  Punt: " << pctPunt << "%" << std::endl;
    std::cout << "  TD (Missed XP/Failed 2pt): " << pctTDMissXP2PtFail << "%" << std::endl;
    std::cout << "  TD (Successful XP): " << pctTDSuccessXP << "%" << std::endl;
    std::cout << "  TD (Successful 2pt): " << pctTDSuccess2Pt << "%" << std::endl;
    std::cout << "  End of Half: " << pctEndOfHalf << "%" << std::endl;
    std::cout << "Defense:" << std::endl;
    std::cout << "  Defensive Rating: " << defensiveRating << std::endl;
    std::cout << "---------------------------------------" << std::endl;
}

// Making a change here
//Making another change here