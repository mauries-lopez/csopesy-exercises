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

    ////First-Come First Serve Algorithm
    //// Execute schedulerQueue by getting the first who entered the processList.
    //// Iterate to check for available CPU cores
    ////while (!processList.empty()) {
    //    for (int i : cores) { // iterate through the 4 CPU cores to see if available
    //        // Lock mutex
    //        //std::lock_guard<std::mutex> lock(mtx);
    //        //mtx.lock();
    //        if (cores[i] == -1) { // Free, core can be used
    //            cores[i] = 1; // set core to In Use
    //            // Start the process
    //            // call incrementLine from Process class here
    //            std::thread processIncrementLine(&Process::incrementLine, processList.front());  //Start the Process
    //            processIncrementLine.detach(); //Separate the thread of the process.

    //            // Wait for the process to finish ; might be unnecessary since this is implemented in incrementLine() in Process
    //            /*while (!processList[0]->isFinished()) {
    //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //            }*/

    //            // Check if process in cores[index] is finished
    //            // Include condition ??? but incrementLine should finish before it reaches this part of code
    //            cores[i] = -1;

    //            // Remove process at index 0 in processList
    //            processList.erase(processList.begin());
    //            //break;
    //        }
    //        //mtx.unlock();
    //    }
    ////}

    //First-Come First Serve Algorithm
    // Find any available cores

    int i = 0;
    while (true) {

        std::cout << "ScheduleWorker Running..." << std::endl;;

        // If all cores are checked, recheck all again.
        if (i == cores.size()) {
            i = 0;
        }

        if (cores[i] == -1) { // Found available core
            std::cout << "Found Free Core at Core #" + std::to_string(i) << std::endl;

            ////Sleep(1000);
            if (!processList.empty()) {
                std::thread processIncrementLine(&Process::incrementLine, processList.front());  //Start the Process
                processIncrementLine.detach(); //Separate the thread of the process.
                //processList.erase(processList.begin());
                cores[i] = 1; // Set core to busy
            }
        }

        i++;
        Sleep(3000);
    }

    //std::thread processIncrementLine(&Process::incrementLine, processList.front());  //Start the Process
    //processIncrementLine.detach(); //Separate the thread of the process.
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



//void ScheduleWorker::removeFinishedProcess() {
//    if (!processList.empty()) {
//        processList.erase(processList.begin());
//    }
//}