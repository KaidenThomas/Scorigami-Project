// HalfTimeInfo.h
#ifndef HALFTIMEINFO_H
#define HALFTIMEINFO_H

#include <string> // Required for std::string

// Struct to hold the results of the time conversion
struct HalfTimeInfo {
    int secondsLeftInHalf;
    int currentHalf; // 1 for first half, 2 for second half
};

// Function declaration for getHalfTimeDetails
HalfTimeInfo getHalfTimeDetails(const std::string& currentTimeStr, int currentQuarter);

#endif // HALFTIMEINFO_H
