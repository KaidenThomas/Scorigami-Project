#include <iostream>
#include <string>
#include <vector> // required for std::vector
#include "team.h"
#include "activegame.h"
#include "halftimeinfo.h"
#include "simulatedrive.h"
#include "simulategame.h"
#include "score.h"
#include "fetchmatrix.h" // include fetchmatrix header

int main() {
    // --- 1. define team statistics (distinct for ohio state and notre dame) ---

    // ohio state (osu) - top-tier team metrics
    double osushortestoff = 40.0;   // explosive, quick drives
    double osulongestoff = 220.0;   // ~3.6 minutes
    double osuavgoff = 140.0;       // ~2.3 minutes
    double osupctto = 7.0;          // very low turnover rate
    double osupctos = 0.3;          // extremely low opponent safety chance
    double osupctfg = 10.0;         // lower field goal rate (prefer tds)
    double osupctp = 20.0;          // lower punt rate
    double osupcttdmiss = 3.0;      // very low missed xp/failed 2pt td rate
    double osupcttdxp = 50.0;       // high successful xp td rate
    double osupcttd2pt = 9.7;       // higher successful 2pt td rate (total 100%)
    double osupcteoh = 0.0;
    int osudefenserank = 1;         // "juggernaut" defense

    // notre dame (nd) - high-tier independent team metrics
    double ndshortestoff = 55.0;    // solid, efficient drives
    double ndlongestoff = 280.0;    // ~4.6 minutes
    double ndavgoff = 170.0;        // ~2.8 minutes
    double ndpctto = 10.0;          // moderate turnover rate
    double ndpctos = 0.5;           // low opponent safety chance
    double ndpctfg = 15.0;          // moderate field goal rate
    double ndpctp = 25.0;           // moderate punt rate
    double ndpcttdmiss = 5.0;       // moderate missed xp/failed 2pt td rate
    double ndpcttdxp = 40.0;        // good successful xp td rate
    double ndpcttd2pt = 4.5;        // moderate successful 2pt td rate (total 100%)
    double ndpcteoh = 0.0;
    int nddefenserank = 10;         // "terrifying" defense

    // create team objects with their specific metrics
    Team ohiostate("ohio state",
        osushortestoff, osulongestoff, osuavgoff,
        osupctto, osupctos, osupctfg, osupctp,
        osupcttdmiss, osupcttdxp, osupcttd2pt, osupcteoh,
        osudefenserank);

    Team notredame("notre dame",
        ndshortestoff, ndlongestoff, ndavgoff,
        ndpctto, ndpctos, ndpctfg, ndpctp,
        ndpcttdmiss, ndpcttdxp, ndpcttd2pt, ndpcteoh,
        nddefenserank);

    // --- 2. set up initial game state for national championship (osu vs nd, 2025) ---
    std::cout << "--- initializing game state for national championship simulation ---" << std::endl;

    // game starts q4 7:30
    std::string currenttimestr = "07:30";
    int currentquarter = 4; // fourth quarter
    HalfTimeInfo initialhalftimeinfo = getHalfTimeDetails(currenttimestr, currentquarter);

    // initial scores: osu 31, nd 15
    int initialhomescore = 31; // ohio state's score
    int initialawayscore = 15; // notre dame's score

    // assuming ohio state (home team) and notre dame (away team).
    // for a 4th quarter start, assume the team that is currently trailing gets possession,
    // or if it's a fresh quarter start, it depends on who received the opening kickoff.
    // let's assume notre dame (away) has possession as they are trailing.
    bool hometeamreceivedkickoff = true; // this affects 3rd quarter kickoff, but not directly 4th quarter start
    std::string initialpossessionteamname = notredame.getTeamName();

    // create the activegame instance
    ActiveGame currentgamestate(ohiostate, notredame, hometeamreceivedkickoff,
        initialhomescore, initialawayscore,
        initialpossessionteamname,
        initialhalftimeinfo);

    // --- 3. call fetchmatrix to run simulations and populate the matrix ---
    // the returned matrix is now stored in 'resultmatrix'
    std::vector<std::vector<int>> resultmatrix = fetchMatrix(currentgamestate);

    // you can now use 'resultmatrix' for further analysis if needed.
    // for example, you could pass it to another function or process it here.

    return 0;
}