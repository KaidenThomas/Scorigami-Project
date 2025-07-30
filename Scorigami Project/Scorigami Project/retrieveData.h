// retrieveData.h
#ifndef RETRIEVEDATA_H
#define RETRIEVEDATA_H

#include <vector>   // Required for std::vector
#include <string>   // Required for std::string
#include <map>      // Required for std::map
#include "ActiveGame.h" // Required for ActiveGame class

// Function declaration for retrieveData
// This function takes the score matrix from fetchMatrix, the current game state,
// and the total number of simulations, then writes relevant game information
// and top score combinations (as percentages) to currentData.txt.
void retrieveData(const std::vector<std::vector<int>>& scoreMatrix,
    const ActiveGame& currentGameState,
    int numSimulations); // Added numSimulations parameter

#endif // RETRIEVEDATA_H
