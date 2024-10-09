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
   
    std::thread processIncrementLine(&Process::incrementLine, processList[0]);  //Start the Process
    processIncrementLine.detach(); //Separate the thread of the process.
}

void ScheduleWorker::displaySchedule() const {
    std::cout << "Scheduled Processes:" << std::endl;
    for (const auto& process : processList) {
        std::cout << " - " << process->processName << std::endl;
    }
}
