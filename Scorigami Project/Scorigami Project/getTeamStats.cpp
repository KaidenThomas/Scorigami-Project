#include "getTeamStats.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::pair<Team, Team> getTeamStats(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open stats file: " + filepath);
    }

    auto readTeam = [&](std::istream& in) -> Team {
        std::string name;
        double shortestOff, longestOff, avgOff;
        double pctTO, pctOS, pctFG, pctP, pctTDMiss, pctTDXP, pctTD2Pt, pctEOH;
        int defenseRank;

        std::getline(in, name);
        in >> shortestOff >> longestOff >> avgOff
            >> pctTO >> pctOS >> pctFG >> pctP
            >> pctTDMiss >> pctTDXP >> pctTD2Pt >> pctEOH
            >> defenseRank;
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip to next line

        return Team(name, shortestOff, longestOff, avgOff,
            pctTO, pctOS, pctFG, pctP,
            pctTDMiss, pctTDXP, pctTD2Pt, pctEOH,
            defenseRank);
        };

    Team awayTeam = readTeam(file);
    Team homeTeam = readTeam(file);

    return std::make_pair(awayTeam, homeTeam);
}