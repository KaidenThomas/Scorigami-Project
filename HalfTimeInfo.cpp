// HalfTimeInfo.cpp
#include "HalfTimeInfo.h" // Include its own header first
#include <stdexcept> // For std::invalid_argument
#include <iostream>  // For std::cerr (if you keep warnings)

// Function definition for getHalfTimeDetails
HalfTimeInfo getHalfTimeDetails(const std::string& currentTimeStr, int currentQuarter) {
    int minutes = 0;
    int seconds = 0;
    size_t colonPos = currentTimeStr.find(':');

    if (colonPos == std::string::npos || colonPos == 0 || colonPos == currentTimeStr.length() - 1) {
        throw std::invalid_argument("Invalid time format. Expected MM:SS.");
    }

    try {
        minutes = std::stoi(currentTimeStr.substr(0, colonPos));
        seconds = std::stoi(currentTimeStr.substr(colonPos + 1));
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Failed to parse time: " + std::string(e.what()));
    }

    if (minutes < 0 || minutes > 15 || seconds < 0 || seconds >= 60 || (minutes == 15 && seconds > 0)) {
        throw std::invalid_argument("Invalid minutes or seconds in time string. Minutes must be 0-15, seconds 0-59.");
    }

    int secondsInCurrentQuarter = minutes * 60 + seconds;
    HalfTimeInfo info;
    const int QUARTER_DURATION_SECONDS = 15 * 60; // 900 seconds

    switch (currentQuarter) {
    case 1:
        info.currentHalf = 1;
        info.secondsLeftInHalf = secondsInCurrentQuarter + QUARTER_DURATION_SECONDS;
        break;
    case 2:
        info.currentHalf = 1;
        info.secondsLeftInHalf = secondsInCurrentQuarter;
        break;
    case 3:
        info.currentHalf = 2;
        info.secondsLeftInHalf = secondsInCurrentQuarter + QUARTER_DURATION_SECONDS;
        break;
    case 4:
        info.currentHalf = 2;
        info.secondsLeftInHalf = secondsInCurrentQuarter;
        break;
    default:
        throw std::invalid_argument("Invalid quarter. Must be 1, 2, 3, or 4.");
    }
    return info;
}
