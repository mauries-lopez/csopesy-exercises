#include "Process.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include <iostream>
#include <thread>
#include <mutex>


Process::Process(const std::string& name, int id, int totalLines, const std::string& timeCreated)
    : processName(name), processID(id), totalLineOfInstruction(totalLines),
    currLineOfInstruction(0), timeCreated(timeCreated) {}

// Constructor to initialize the process with a name, ID, and other properties
//Process::Process(String name, int id, int totalLineOfInstruction, String timeCreated) {
//	this->processName = name;
//	this->processID = id;
//	this->currLineOfInstruction = 0;
//	this->totalLineOfInstruction = totalLineOfInstruction;
//	this->timeCreated = timeCreated;
//}

void Process::incrementLine() {

    ConsoleManager::getInstance()->unfinishedProcessList.push_back(this);

    for (int i = this->currLineOfInstruction; i <= this->totalLineOfInstruction; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        std::string log = "Hello World from " + processName; // Create Print Statement
        printLogs.push_back(log); // Put print statement to printLogs
        this->currLineOfInstruction = i;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    if (this->isFinished()) {
        
        // If process is finish, let core be available again
        // Find any 1 within the cores then make it available again.
        for (int i = 0; i < ScheduleWorker::cores.size(); i++) {
            if (ScheduleWorker::cores[i] == 1) {
                ScheduleWorker::cores[i] = -1;
                break;
            }
        }

        ConsoleManager::getInstance()->addFinishedProcess(this);
    }
    
}

std::vector<std::string> Process::getPrintLogs() {
    if (!printLogs.empty()) {
        return printLogs;
    }
    return {};
}