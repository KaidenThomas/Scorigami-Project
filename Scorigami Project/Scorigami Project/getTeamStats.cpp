#include "getTeamStats.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <numeric>

std::pair<Team, Team> getTeamStats(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open stats file: " + filepath);
    }

    auto readTeam = [&](std::istream& in) -> Team {
        std::string name;
        double shortestOff, longestOff, avgOff;
        double pctTO, pctTOScore, pctOS, pctFG, pctP, pctTDMiss, pctTDXP, pctTD2Pt, pctEOH;
        int defenseRank;

        std::getline(in, name);
        in >> shortestOff >> longestOff >> avgOff
            >> pctTO >> pctTOScore >> pctOS >> pctFG >> pctP
            >> pctTDMiss >> pctTDXP >> pctTD2Pt >> pctEOH
            >> defenseRank;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip to next line

        // Add turnover score percentage to turnover percentage
        pctTO += pctTOScore;

        // Normalize all drive outcome probabilities so they sum to 1
        std::vector<double> outcomes = {
            pctTO, pctOS, pctFG, pctP,
            pctTDMiss, pctTDXP, pctTD2Pt, pctEOH
        };
        double total = std::accumulate(outcomes.begin(), outcomes.end(), 0.0);
        if (total == 0.0) {
            throw std::runtime_error("Sum of drive outcome probabilities is zero for team: " + name);
        }
        for (auto& v : outcomes) v /= total;

        // Assign normalized values back
        pctTO = outcomes[0];
        pctOS = outcomes[1];
        pctFG = outcomes[2];
        pctP = outcomes[3];
        pctTDMiss = outcomes[4];
        pctTDXP = outcomes[5];
        pctTD2Pt = outcomes[6];
        pctEOH = outcomes[7];

        return Team(name, shortestOff, longestOff, avgOff,
            pctTO, pctOS, pctFG, pctP,
            pctTDMiss, pctTDXP, pctTD2Pt, pctEOH,
            defenseRank);
        };

    Team awayTeam = readTeam(file);
    Team homeTeam = readTeam(file);

    return std::make_pair(awayTeam, homeTeam);
}