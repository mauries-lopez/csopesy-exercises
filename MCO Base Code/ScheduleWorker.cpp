#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

// Initialize cores
int cores[4] = {-1,-1,-1,-1}; // array of cores size 4, each initialized as -1 (free; set to 1 if in use)

ScheduleWorker::ScheduleWorker() {

}

ScheduleWorker::~ScheduleWorker() {

}

void ScheduleWorker::addProcess(std::shared_ptr<Process> process) {
    
    processList.push_back(process);
}

void ScheduleWorker::runProcess(int core) {
    if (core == -1) { // available
        core = 1; // In use
        std::thread processIncrementLine(&Process::incrementLine, processList[0]);  //Start the Process
        processIncrementLine.detach(); //Separate the thread of the process.
        core = -1; // Available
    }
}

void ScheduleWorker::executeProcess() {

    //First-Come First Serve Algorithm
    // Execute schedulerQueue by getting the first who entered the processList.
    while (!processList.empty()) {
        // Iterate to check for available CPU cores (4)
        for (int i : cores) {
            std::thread core(runProcess, i);
            core.detach();
        }
    }
}

void ScheduleWorker::displaySchedule() const {
    std::cout << "Scheduled Processes:" << std::endl;
    for (const auto& process : processList) {
        std::cout << " - " << process->processName << std::endl;
    }
}

//void ScheduleWorker::removeFinishedProcess() {
//    if (!processList.empty()) {
//        processList.erase(processList.begin());
//    }
//}