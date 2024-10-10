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

void ScheduleWorker::executeProcess() {

    //First-Come First Serve Algorithm
    // Execute schedulerQueue by getting the first who entered the processList.
    // Iterate to check for available CPU cores
    //while (!processList.empty()) {
        for (int i : cores) { // iterate through the 4 CPU cores to see if available
            // Lock mutex
            //std::lock_guard<std::mutex> lock(mtx);
            //mtx.lock();
            if (cores[i] == -1) { // Free, core can be used
                cores[i] = 1; // set core to In Use
                // Start the process
                // call incrementLine from Process class here
                std::thread processIncrementLine(&Process::incrementLine, processList.front());  //Start the Process
                processIncrementLine.detach(); //Separate the thread of the process.

                // Wait for the process to finish ; might be unnecessary since this is implemented in incrementLine() in Process
                /*while (!processList[0]->isFinished()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }*/

                // Check if process in cores[index] is finished
                // Include condition ??? but incrementLine should finish before it reaches this part of code
                cores[i] = -1;

                // Remove process at index 0 in processList
                processList.erase(processList.begin());
                //break;
            }
            //mtx.unlock();
        }
    //}
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