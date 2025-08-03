#include "getScorigamiHistory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

std::vector<std::vector<bool>> getScorigamiHistory(const std::string& filepath) {
    // Initialize 200x200 matrix to false
    std::vector<std::vector<bool>> matrix(200, std::vector<bool>(200, false));

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open scorigami info file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) continue;

        // Parse format "away,home"
        size_t comma = line.find(',');
        if (comma == std::string::npos) continue;

        int away = std::stoi(line.substr(0, comma));
        int home = std::stoi(line.substr(comma + 1));

        // Determine winner and loser
        int winner = std::max(away, home);
        int loser = std::min(away, home);

        // Only mark if within bounds and not a tie
        if (winner >= 0 && winner < 200 && loser >= 0 && loser < 200 && winner != loser) {
            matrix[loser][winner] = true; // <-- FLIPPED AXES HERE
        }
    }

    return matrix;
}