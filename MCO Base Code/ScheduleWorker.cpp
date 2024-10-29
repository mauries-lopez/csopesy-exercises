#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <Windows.h>
#include "MainConsole.h"

std::vector<std::shared_ptr<Process>> ScheduleWorker::processList;
std::vector<std::shared_ptr<Process>> ScheduleWorker::waitingQueue;

ScheduleWorker::ScheduleWorker() {

}

ScheduleWorker::~ScheduleWorker() {

}

void ScheduleWorker::initialize(int numCores) {

    this->schedulerCurCycle = MainConsole::curClockCycle;

    this->availableCores = numCores;
    this->initializeCores(numCores);

    //Make a thread for the scheduler so it can constantly check for processes
    std::thread scheduleThread(&ScheduleWorker::scheduleProcess, this);

    //Detach it
    scheduleThread.detach();
}

void ScheduleWorker::addProcess(std::shared_ptr<Process> process) {
    processList.push_back(process);
}

void ScheduleWorker::addWaitProcess(std::shared_ptr<Process> process) {
    waitingQueue.push_back(process);
}

void ScheduleWorker::scheduleProcess() {

    // Pause for a moment (This is necessary so that it will start checking on CPU #0 upon initialized)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    //First-Come First Serve Algorithm
    int i = 0;
    while (true) {

        if (this->schedulerCurCycle != MainConsole::curClockCycle) {
            // If all cores are checked, recheck all again.
            if (i == cores.size()) {
                i = 0;
            }
            if (!processList.empty()) {
                if (cores[i] == -1) { // Found available core
                    // Assign core to process
                    coreAssigned = i;
                    //Set core to busy
                    cores[i] = 1;
                    // Add count of used cores
                    usedCores++;
                    // Associate core to currently executed process
                    //Start the Process
                    std::thread processIncrementLine(&Process::incrementLine, processList.front(), coreAssigned);
                    //Separate the thread of the process.
                    processIncrementLine.detach();
                    //Remove the current process in processList <-- processList should be empty again 
                    processList.erase(processList.begin());
                    // Add to processList the process at the top of waitingQueue
                    if (!waitingQueue.empty()) {
                        processList.push_back(waitingQueue.front());
                    }
                }
            }

            i++;
            this->schedulerCurCycle = MainConsole::curClockCycle;
            Sleep(100);
        }

    }
}

void ScheduleWorker::initializeCores(int numCores) {
    for (int i = 0; i < numCores; i++) {
        cores.push_back(-1);
    }
}

void ScheduleWorker::displaySchedule() const {
    std::cout << "Scheduled Processes:" << std::endl;
    for (const auto& process : processList) {
        std::cout << " - " << process->processName << std::endl;
    }
}