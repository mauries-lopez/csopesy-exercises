#include "Utils.h"
#include <string>
#include <ctime>

std::string formatTime(const time_t& timeValue) {
    char timeCreation[50];
    struct tm datetime;
    localtime_s(&datetime, &timeValue); 
    strftime(timeCreation, 50, "%m/%d/%Y, %I:%M:%S%p", &datetime); 
    return std::string(timeCreation); 
}

