#pragma once
#include "Utils.h"
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <mutex>

class Process {
public:
    // Constructor to initialize the process with a name, ID, total lines of instruction, and time created
    Process(const std::string& name, int id, int totalLines, const std::string& timeCreated);
    ~Process() = default;

    // Method to check if the process is finished
    bool isFinished() const {
        return currLineOfInstruction >= totalLineOfInstruction;
    }

    // Method to increment the current line of instruction
    void incrementLine();

    void setStartTime(const time_t& startTime) {
        this->startTime = startTime;
    }
    time_t startTime;
    time_t getStartTime() const {
        return startTime;
    }
    
    // Getters
    std::string getName() const { return processName; }
    int getID() const { return processID; }
    int getCurrentLine() const { return currLineOfInstruction; } 
    int getTotalLines() const { return totalLineOfInstruction; } 
    std::vector<std::string> getPrintLogs();
    std::string getTimeCreated() const { return timeCreated; }
    std::string processName;
    int getCoreID() const { return coreID; }

private:
    //std::string processName;          // Name of the process
    std::mutex mtx;                     // Mutex for thread safety
    int coreID;
    int processID;                      // ID of the process
    int currLineOfInstruction;          // Current line of instruction
    int totalLineOfInstruction;         // Total lines of instruction
    std::string timeCreated;            // Time when the process was created

    // Temporary (can be deleted later on)
    std::vector<std::string> printLogs;

    // Instance of ScheduleWorker
    //ScheduleWorker* scheduleWorker;
};