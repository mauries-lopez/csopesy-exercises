#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>

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
    while (true) {
        /*for (size_t index : cores) {
            if(coreList[index])
        }*/

        // if core 0 is empty, get and execute the process at the top of the queue of processList
        std::thread processIncrementLine(&Process::incrementLine, processList[0]);  //Start the Process
        processIncrementLine.detach(); //Separate the thread of the process.

        // if core 1 is empty, get and execute the process at the top of the queue of processList

        // if core 2 is empty, get and execute the process at the top of the queue of processList

        // if core 3 is empty, get and execute the process at the top of the queue of processList

    }
   
    
}

void ScheduleWorker::displaySchedule() const {
    std::cout << "Scheduled Processes:" << std::endl;
    for (const auto& process : processList) {
        std::cout << " - " << process->processName << std::endl;
    }
}
