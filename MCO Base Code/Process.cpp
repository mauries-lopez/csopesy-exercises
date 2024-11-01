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

    while (true) {
        if (this->processCurCycle != MainConsole::curClockCycle) {
            if (currLineOfInstruction >= totalLineOfInstruction) {
                break;
            }

            std::lock_guard<std::mutex> lock(mtx);

            time_t currTime;
            char timeCreation[50];
            struct tm datetime;
            time(&currTime);
            localtime_s(&datetime, &currTime);
            strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

            this->setCoreAssigned(core);

            currLineOfInstruction++;
            this->processCurCycle = MainConsole::curClockCycle;
        }
    }

    if (currLineOfInstruction >= totalLineOfInstruction) {
        ScheduleWorker::cores[coreAssigned] = -1;
        ConsoleManager::getInstance()->addFinishedProcess(this);
        FileWrite::generateFile(processID, processName, getTimeCreated(), printLogs);
        ScheduleWorker::usedCores--;
    }
}

void Process::processSMI() const {
    std::cout << "Process Name: " << processName << std::endl;
    std::cout << "ID: " << processID << std::endl;
    std::cout << "Current Line of Instruction: " << currLineOfInstruction << "/" << totalLineOfInstruction << std::endl;

    if (currLineOfInstruction >= totalLineOfInstruction) {
        std::cout << "Status: Finished" << std::endl;
    }
    else {
        std::cout << "Status: Running" << std::endl;
    }
}


std::vector<std::string> Process::getPrintLogs() {
    if (!printLogs.empty()) {
        return printLogs;
    }
    return {};
}