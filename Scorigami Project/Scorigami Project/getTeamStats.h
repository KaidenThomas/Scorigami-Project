#pragma once
#include <string>
#include <utility>
#include "team.h"

// Reads team stats from a file and returns a pair: {awayTeam, homeTeam}
std::pair<Team, Team> getTeamStats(const std::string& filepath);