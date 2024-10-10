#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>

// Initialize cores
int cores[4] = {-1,-1,-1,-1}; // array of cores size 4, each initialized as -1 (free; set to 1 if in use)

ScheduleWorker::ScheduleWorker() {

}

ScheduleWorker::~ScheduleWorker() {

}

void ScheduleWorker::addProcess(std::shared_ptr<Process> process) {
    
    processList.push_back(process);
}

void ScheduleWorker::executeProcess() {

    //First-Come First Serve Algorithm
    // Execute schedulerQueue by getting the first who entered the processList.
    //while (true) {
        for (int index : cores) {
            if (cores[index] == -1) { // Free, core can be used
                cores[index] = 1; // set core to In Use
                // Start the process
                // call incrementLine from Process class here
                std::thread processIncrementLine(&Process::incrementLine, processList[0]);  //Start the Process
                processIncrementLine.detach(); //Separate the thread of the process.

                // Check if process in cores[index] is finished
                //if (this->currLineOfInstruction == 100) {
                    cores[index] = -1;
                //}
            }
        }
    //}
}

void ScheduleWorker::displaySchedule() const {
    std::cout << "Scheduled Processes:" << std::endl;
    for (const auto& process : processList) {
        std::cout << " - " << process->processName << std::endl;
    }
}
