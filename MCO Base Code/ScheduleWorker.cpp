#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <Windows.h>
// Initialize cores
//int cores[4] = {-1,-1,-1,-1}; // array of cores size 4, each initialized as -1 (free; set to 1 if in use)

std::vector<std::shared_ptr<Process>> ScheduleWorker::processList;

ScheduleWorker::ScheduleWorker() {

}

ScheduleWorker::~ScheduleWorker() {

}

void ScheduleWorker::initialize(int numCores) {
    this->initializeCores(numCores);

    //Make a thread for the scheduler so it can constantly check for processes
    std::thread scheduleThread(&ScheduleWorker::scheduleProcess, this);

    //Detach it
    scheduleThread.detach();
}

void ScheduleWorker::addProcess(std::shared_ptr<Process> process) {
    processList.push_back(process);
}

void ScheduleWorker::scheduleProcess() {

    //First-Come First Serve Algorithm
    int i = 0;
    while (true) {

        // If all cores are checked, recheck all again.
        if (i == cores.size()) {
            i = 0;
        }

        if (!processList.empty()) {
            if (cores[i] == -1) { // Found available core
                //Start the Process
                std::thread processIncrementLine(&Process::incrementLine, processList.front());
                //Separate the thread of the process.
                processIncrementLine.detach();
                //Remove the current process in processList <-- processList should be empty again 
                processList.erase(processList.begin());
                //Set core to busy
                cores[i] = 1;
            }
        }

        i++;
        Sleep(100); // maybe can help to avoid cpu overuse
    }
}

void ScheduleWorker::initializeCores(int numCores) {
    for (int i = 0; i < numCores; i++) {
        cores.push_back(-1);
    }
}



//void ScheduleWorker::removeFinishedProcess() {
//    if (!processList.empty()) {
//        processList.erase(processList.begin());
//    }
//}