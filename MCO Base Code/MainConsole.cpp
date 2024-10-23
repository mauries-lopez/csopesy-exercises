#include <iostream>
#include "TypedefRepo.h"
#include "MainConsole.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include "Process.h"
#include "AConsole.h"
#include <sstream>
#include <ctime>
#include <thread>
#include <fstream>

using namespace std; //To not specify the prefix (std::<syntax>)

namespace fs = std::filesystem;

int processID = 0; // This could be generated dynamically
std::vector <std::string> processesNameList;
bool isInitialized = false;

// Core Related
std::vector<int> ScheduleWorker::cores;
bool anyAvailableCore = false;
bool testRun = false;

void asciiPrint() {
    string asciiText[6] = { "  _____  _____  ____  _____  ______  _______     __",
                            " / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / /",
                            "| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ / ",
                            "| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /  ",
                            "| |____ ____) | |__| | |    | |____ ____) |  | |   ",
                            " \\_____|_____/ \\____/|_|    |______|_____/   |_|   " };

    for (int i = 0; i < 6; i++) {
        cout << asciiText[i] << "\n";
    }
}

MainConsole::MainConsole(String name) : AConsole(name) {}

MainConsole::~MainConsole() {}

void MainConsole::onEnabled() {
    this->refresh = false;
}

void MainConsole::display() {
    
}

void MainConsole::process() {

    if (this->refresh == false) {
        asciiPrint();
        this->refresh = true;
    }
    
    //for (int i = 0; i < 10; i++) {
    //    if (!testRun) {
    //        // Get current time
    //        time_t currTime;
    //        char timeCreation[50];
    //        struct tm datetime;
    //        time(&currTime);
    //        localtime_s(&datetime, &currTime);
    //        strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

    //        string timeCreated = (string)timeCreation;

    //        std::string processName = "process" + std::to_string(i);
    //        shared_ptr<Process> process = make_shared<Process>(processName, i, 100, timeCreated);

    //        processesNameList.push_back(processName); //Para di matake ulit ung name

    //        shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);
    //        ConsoleManager::getInstance()->registerScreen(baseScreen);

    //        // Check for available cores
    //        for (int i = 0; i < ScheduleWorker::cores.size(); i++) {
    //            if (ScheduleWorker::cores[i] == -1) {
    //                anyAvailableCore = true;
    //                break;
    //            }
    //            else {
    //                anyAvailableCore = false;
    //            }
    //        }

    //        // addProcess if there is available core
    //        if (anyAvailableCore) {
    //            ScheduleWorker::addProcess(process);
    //        }
    //        else { // add to waiting queue if no available core
    //            ScheduleWorker::addWaitProcess(process);
    //        }
    //    }
    //}
    //
    //testRun = true;

    string main_CommandInput = "";

    //// Initialize a vector to hold active screens
    vector<shared_ptr<BaseScreen>> activeScreens;

    // Ask for user input
    cout << "@MainConsole->: ";
    getline(cin, main_CommandInput);

    // Extract command into tokens
    istringstream stream(main_CommandInput);
    string command, prefix, processName;
    stream >> command >> prefix >> processName;
    
    if (isInitialized == false) {
        if (command == "initialize") {

            isInitialized = true;

            // Read Config.txt
            std::ifstream input("config.txt");
            string parameter, value;

            while (std::getline(input, parameter)) {
                istringstream stream(parameter);
                stream >> parameter >> value;

                if (parameter == "num-cpu") { // Set num-cpu
                    // Convert String to Int
                    int numCpu = std::stoi(value);

                    if (numCpu >= 1 && numCpu <= 128) {
                        ScheduleWorker scheduleWorker;
                        scheduleWorker.initialize(numCpu);
                    }
                    else {
                        std::cerr << "The given number of CPU is in the invalid range. The range is [1,128] only." << std::endl;
                        isInitialized = false;
                    }

                }
                else if (parameter == "scheduler") {
                    
                }
                else if (parameter == "quantum-cycles") {

                }
                else if (parameter == "batch-process-freq") {
                    long long frequency = std::stoll(value);

                    if (frequency >= 1 && frequency <= 4294967296) {
                        if (frequency == 1) {
                            // TO-DO:: New Process is Generated at the end of each CPU cycle.
                            this->batchProcessFreq = frequency;
                        }
                        else {
                            this->batchProcessFreq = frequency;
                        }
                    }
                    else {
                        std::cerr << "The given number of Batch Process Frequency is in the invalid range. The range is [1,2^32] only." << std::endl;
                        isInitialized = false;
                    }
                    
                }
                else if (parameter == "min-ins") {
                    long long minIns = std::stoll(value);

                    if (minIns >= 1 && minIns <= 4294967296) {
                        this->minimumIns = minIns;
                    }
                    else {
                        std::cerr << "The given number of Minimum Instruction/s is in the invalid range. The range is [1,2^32] only." << std::endl;
                        isInitialized = false;
                    }
                    
                }
                else if (parameter == "max-ins") {
                    long long maxIns = std::stoll(value);

                    if (maxIns >= 1 && maxIns <= 4294967296) {
                        this->maximumIns = maxIns;
                    }
                    else {
                        std::cerr << "The given number of ,aximum Instruction/s is in the invalid range. The range is [1,2^32] only." << std::endl;
                        isInitialized = false;
                    }
                }
                else if (parameter == "delays-per-exec") {
                    long long delays = std::stoll(value);

                    if (delays >= 0 && delays <= 4294967296) {

                        if (delays == 0) {
                            // TO-DO:: If delay is 0, each instruction is executed per CPU cycle
                            this->delaysPerExec = delays;
                        }
                        else {
                            this->delaysPerExec = delays;
                        }
                    }
                    else {
                        std::cerr << "The given number of Delays Per Execution is in the invalid range. The range is [1,2^32] only." << std::endl;
                        isInitialized = false;
                    }
                }
            }
            std::cout << this->batchProcessFreq << std::endl;
            std::cout << this->delaysPerExec << std::endl;
            std::cout << this->maximumIns << std::endl;
            std::cout << this->minimumIns << std::endl;

        }
        else if (command == "exit") {
            ConsoleManager::getInstance()->exitApplication();
        }
        else {
            std::cerr << "System is not yet initialized. Initialize first." << std::endl;
        }

    }
    else {

        if (isInitialized == true) {
            // Check for specific command formats
            if (command == "screen") {

                if (prefix == "-s" && !processName.empty()) {

                    //If there are any available cores, proceed. Else, error.
                    //Check if there are any available cores
                    for (int i = 0; i < ScheduleWorker::cores.size(); i++) {
                        if (ScheduleWorker::cores[i] == -1) {
                            anyAvailableCore = true;
                            break;
                        }
                        else {
                            anyAvailableCore = false;
                        }
                    }

                    if (anyAvailableCore == true) {
                        // Get current time
                        time_t currTime;
                        char timeCreation[50];
                        struct tm datetime;
                        time(&currTime);
                        localtime_s(&datetime, &currTime);
                        strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

                        string timeCreated = (string)timeCreation;

                        // Line 84 to 90: This is used for PRE-DETERMINING if the process name is already initialized or not.
                        // This is to avoid the unnecessary creation of process.
                        bool isProcessNameAvailable = true;
                        for (int i = 0; i < processesNameList.size(); i++) {
                            if (processName == processesNameList[i]) {
                                isProcessNameAvailable = false;
                            }
                        }

                        if (isProcessNameAvailable) {
                            processesNameList.push_back(processName);

                            shared_ptr<Process> process = make_shared<Process>(processName, processID++, 100, timeCreated);
                            shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);
                            ConsoleManager::getInstance()->registerScreen(baseScreen);

                            ScheduleWorker::addProcess(process);

                            //std::thread baseScreenInputThread(&ScheduleWorker::executeProcess, &scheduleWorker);
                            ////scheduleWorker.executeProcess();

                            ConsoleManager::getInstance()->switchConsole(processName);

                            activeScreens.push_back(baseScreen);
                        }
                        else {
                            std::cerr << "Screen name " << processName << " already exists. Please use a different name." << std::endl;
                        }
                    }
                    else {
                        std::cerr << "No Available Core. Process is queued." << std::endl;
                        // Add process to waiting queue
                        // Assigning process
                        // Get current time
                        time_t currTime;
                        char timeCreation[50];
                        struct tm datetime;
                        time(&currTime);
                        localtime_s(&datetime, &currTime);
                        strftime(timeCreation, 50, "%m/%d/%G, %r", &datetime);

                        string timeCreated = (string)timeCreation;

                        bool isProcessNameAvailable = true;
                        for (int i = 0; i < processesNameList.size(); i++) {
                            if (processName == processesNameList[i]) {
                                isProcessNameAvailable = false;
                            }
                        }

                        if (isProcessNameAvailable) {
                            processesNameList.push_back(processName);

                            shared_ptr<Process> process = make_shared<Process>(processName, processID++, 100, timeCreated);

                            // Add process to waiting queue
                            ScheduleWorker::addWaitProcess(process);
                        }
                        else {
                            std::cerr << "Screen name " << processName << " already exists. Please use a different name." << std::endl;
                        }
                    }

                }

                else if (prefix == "-s" && processName.empty()) {
                    ConsoleManager::getInstance()->nullProcessName();
                }

                else if (prefix == "-r" && !processName.empty()) {
                    ConsoleManager::getInstance()->switchConsole(processName);
                }

                else if (prefix == "-r" && processName.empty()) {
                    ConsoleManager::getInstance()->nullProcessName();
                }

                else if (prefix == "-ls") {
                    ConsoleManager::getInstance()->listFinishedProcesses();
                }

                // improper prefix
                else {
                    ConsoleManager::getInstance()->invalidPrefix();
                }
            } 
            else {
                std::cerr << command + " invalid command." << std::endl;
            }

        }
        
    }


}

