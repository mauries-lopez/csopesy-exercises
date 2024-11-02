#include "ScheduleWorker.h"
#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <Windows.h>
#include "MainConsole.h"
#include <random>

using namespace std;

int testProcessID = 0; // This could be generated dynamically

std::vector<std::shared_ptr<Process>> ScheduleWorker::processList;
std::vector<std::shared_ptr<Process>> ScheduleWorker::waitingQueue;

long long MainConsole::minimumIns = 0;
long long MainConsole::maximumIns = 0;
std::vector <std::string> MainConsole::processesNameList;
bool testAnyAvailableCore = false;
long long MainConsole::batchProcessFreq = 0;


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

void ScheduleWorker::testSchedule() {

    long long createProcessFreq = MainConsole::batchProcessFreq;

    while (!stopTest) {
        if (ScheduleWorker::schedulerCurCycle != MainConsole::curClockCycle) {
            for (long long i = 0; i < createProcessFreq; i++) {
                time_t currTime;
                char timeCreation[50];
                struct tm datetime;
                time(&currTime);
                localtime_s(&datetime, &currTime);
                strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

                std::string timeCreated = (string)timeCreation;

                std::string processName = "autogen_process" + std::to_string(testProcessID);

                bool isProcessNameAvailable = true;
                for (int i = 0; i < MainConsole::processesNameList.size(); i++) {
                    if (processName == MainConsole::processesNameList[i]) {
                        isProcessNameAvailable = false;
                    }
                }

                if (isProcessNameAvailable) {
                    std::random_device rd;
                    std::mt19937_64 gen(rd());  // Use 64-bit version of Mersenne Twister
                    std::uniform_int_distribution<long long> dis(MainConsole::minimumIns, MainConsole::maximumIns);
                    long long random_value = dis(gen);

                    shared_ptr<Process> process = make_shared<Process>(processName, testProcessID, random_value, timeCreated);

                    MainConsole::processesNameList.push_back(processName); //Para di matake ulit ung name

                    shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);
                    ConsoleManager::getInstance()->registerScreen(baseScreen);

                    // Check for available cores
                    for (int i = 0; i < ScheduleWorker::cores.size(); i++) {
                        if (ScheduleWorker::cores[i] == -1) {
                            testAnyAvailableCore = true;
                            break;
                        }
                        else {
                            testAnyAvailableCore = false;
                        }
                    }

                    // addProcess if there is available core
                    if (testAnyAvailableCore) {
                        ScheduleWorker::addProcess(process);
                    }
                    else { // add to waiting queue if no available core
                        ScheduleWorker::addWaitProcess(process);
                    }

                    testProcessID++;
                    ScheduleWorker::schedulerCurCycle = MainConsole::curClockCycle;
                }
                else {
                    std::cerr << "Screen name " << processName << " already exists. Please use a different name." << std::endl;
                }

                
            }
            
        }
    }
    

}