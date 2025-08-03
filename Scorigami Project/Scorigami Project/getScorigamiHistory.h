#pragma once
#include <string>
#include <vector>

// Returns a 200x200 matrix of bools representing all historical score combinations.
// The matrix is indexed as [winner][loser].
std::vector<std::vector<bool>> getScorigamiHistory(const std::string& filepath);