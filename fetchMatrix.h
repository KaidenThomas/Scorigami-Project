// fetchMatrix.h
#ifndef FETCHMATRIX_H
#define FETCHMATRIX_H

#include "ActiveGame.h" // Required for ActiveGame parameter
#include "Score.h"      // Required for Score return type
#include <vector>       // Required for std::vector return type

// Function declaration for fetchMatrix
// Simulates the game multiple times and returns the populated score matrix.
std::vector<std::vector<int>> fetchMatrix(ActiveGame initialGameState);

#endif // FETCHMATRIX_H
