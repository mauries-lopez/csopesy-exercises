#include "Process.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include "FileWrite.h"
#include <iostream>
#include <thread>
#include <mutex>


Process::Process(const std::string& name, int id, int totalLines, const std::string& timeCreated, int coreAssigned)
    : processName(name), processID(id), totalLineOfInstruction(totalLines),
    currLineOfInstruction(0), timeCreated(timeCreated), coreAssigned(coreAssigned) {}

// Constructor to initialize the process with a name, ID, and other properties
//Process::Process(String name, int id, int totalLineOfInstruction, String timeCreated) {
//	this->processName = name;
//	this->processID = id;
//	this->currLineOfInstruction = 0;
//	this->totalLineOfInstruction = totalLineOfInstruction;
//	this->timeCreated = timeCreated;
//}

void Process::incrementLine(int core) {

    ConsoleManager::getInstance()->unfinishedProcessList.push_back(this);

    for (int i = this->currLineOfInstruction; i <= this->totalLineOfInstruction; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        // Get current time
        time_t currTime;
        char timeCreation[50];
        struct tm datetime;
        time(&currTime);
        localtime_s(&datetime, &currTime);
        strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

        this->setCoreAssigned(core);
        std::string timeCreated = "Executed line at timestamp: (" + (std::string)timeCreation + ")"; // timestamp of execution
        std::string coreUsed = "Core: " + std::to_string(coreAssigned); // associated core
        std::string printExec = "Hello World from " + processName; // Create Print Statement (execution)
        std::string log = timeCreated + "   " + coreUsed + "   " + printExec;
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
        FileWrite::generateFile(processID, processName, getTimeCreated(), printLogs);
    }
    
}

std::vector<std::string> Process::getPrintLogs() {
    if (!printLogs.empty()) {
        return printLogs;
    }
    return {};
}