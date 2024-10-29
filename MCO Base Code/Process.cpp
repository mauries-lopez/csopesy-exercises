#include "Process.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include "MainConsole.h"
#include "FileWrite.h"
#include <iostream>
#include <thread>
#include <mutex>


Process::Process(const std::string& name, int id, long long totalLines, const std::string& timeCreated, int coreAssigned)
    : processName(name), processID(id), totalLineOfInstruction(totalLines),
    currLineOfInstruction(0), timeCreated(timeCreated), coreAssigned(coreAssigned) {}

void Process::incrementLine(int core) {

    ConsoleManager::getInstance()->unfinishedProcessList.push_back(this);

    long long i = this->currLineOfInstruction;

    do {
        
        // If process is not sync -> cpu already iterated. Hence, need to do 1 round of work then sync
        if (this->processCurCycle != MainConsole::curClockCycle) {

            // End Case
            if (i > this->totalLineOfInstruction) {
                break;
            }

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
            i++;
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
            this->processCurCycle = MainConsole::curClockCycle;
        }

        
    } while (true);

    if (this->isFinished()) {
        
        // If process is finish, let core be available again
        ScheduleWorker::cores[coreAssigned] = -1;
        ConsoleManager::getInstance()->addFinishedProcess(this);
        FileWrite::generateFile(processID, processName, getTimeCreated(), printLogs);
        ScheduleWorker::usedCores--;

        //TO-DO:: Remove Process' BaseScreen

    }
    
}

std::vector<std::string> Process::getPrintLogs() {
    if (!printLogs.empty()) {
        return printLogs;
    }
    return {};
}