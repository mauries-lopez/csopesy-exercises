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
#include <random>

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

    cout << "__________________________________________________" << endl;
    cout << "\nWelcome to CSOPESY Emulator!" << endl;
    cout << "\nDevelopers: \nLopez, Mauries\nRomblon, Kathleen\nTighe, Kaitlyn\n"; 
    cout << "\nLast updated: 10-XX-2024\n"; // TODO: update when last updated ig
    cout << "__________________________________________________" << endl;

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
                        this->totalNumCores = numCpu;
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
            //std::cout << this->batchProcessFreq << std::endl;
            //std::cout << this->delaysPerExec << std::endl;
            //std::cout << this->maximumIns << std::endl;
            //std::cout << this->minimumIns << std::endl;


            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // 10 Process after Intializing... Temporary
            
            for (int i = 0; i < 10; i++) {
                if (!testRun) {
                    // Get current time
                    time_t currTime;
                    char timeCreation[50];
                    struct tm datetime;
                    time(&currTime);
                    localtime_s(&datetime, &currTime);
                    strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);

                    string timeCreated = (string)timeCreation;

                    std::string processName = "process" + std::to_string(i);

                    std::random_device rd;
                    std::mt19937_64 gen(rd());  // Use 64-bit version of Mersenne Twister
                    std::uniform_int_distribution<long long> dis(this->minimumIns, this->maximumIns);
                    long long random_value = dis(gen);

                    shared_ptr<Process> process = make_shared<Process>(processName, i, random_value, timeCreated);

                    processesNameList.push_back(processName); //Para di matake ulit ung name

                    shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);
                    ConsoleManager::getInstance()->registerScreen(baseScreen);

                    // Check for available cores
                    for (int i = 0; i < ScheduleWorker::cores.size(); i++) {
                        if (ScheduleWorker::cores[i] == -1) {
                            anyAvailableCore = true;
                            break;
                        }
                        else {
                            anyAvailableCore = false;
                        }
                    }

                    // addProcess if there is available core
                    if (anyAvailableCore) {
                        ScheduleWorker::addProcess(process);
                    }
                    else { // add to waiting queue if no available core
                        ScheduleWorker::addWaitProcess(process);
                    }

                }
            }

            testRun = true;
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////

            std::cerr << "System successfully initialized." << std::endl;

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
            if (command == "screen" && !prefix.empty()) {

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

                            //Randomize Number of Maximum Instruction within given interval
                            // Create a random device and a random engine
                            std::random_device rd;
                            std::mt19937_64 gen(rd());  // Use 64-bit version of Mersenne Twister
                            std::uniform_int_distribution<long long> dis(this->minimumIns, this->maximumIns);
                            long long random_value = dis(gen);

                            shared_ptr<Process> process = make_shared<Process>(processName, processID++, random_value, timeCreated);
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

                            //Randomize Number of Maximum Instruction within given interval
                            // Create a random device and a random engine
                            std::random_device rd;
                            std::mt19937_64 gen(rd());  // Use 64-bit version of Mersenne Twister
                            std::uniform_int_distribution<long long> dis(this->minimumIns, this->maximumIns);
                            long long random_value = dis(gen);

                            shared_ptr<Process> process = make_shared<Process>(processName, processID++, random_value, timeCreated);

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
                    ConsoleManager::getInstance()->listFinishedProcesses(false); // set writeToFIle as false
                }

                // improper prefix
                else {
                    ConsoleManager::getInstance()->invalidPrefix();
                }
            }
            else if (command == "screen" && prefix.empty()) {
                //TO-DO : Details about screen. How to use screen commands?
                std::cout << "\n[Screen Command Information]" << std::endl;
                std::cout << "List of Screen Commands:" << std::endl;
                std::cout << "(1) screen -s <process name> \tTo create a new process" << std::endl;
                std::cout << "(2) screen -r <process name> \tTo view the details of an created proces" << std::endl;
                std::cout << "(3) screen -ls \t\t\tTo view unfinish and finish processes\n" << std::endl;
            }
            // Start schedule test
            std::cout << "> Creating dummy processes" << std::endl;
            ConsoleManager::getInstance()->schedulerTest(batchProcessFreq, minimumIns, maximumIns);
            }
            else if (command == "scheduler-stop") {
             // Stop schedule test
                std::cout << "> Stopping creation of dummy processes" << std::endl;
                ConsoleManager::getInstance()->schedulerTestStop();
            }

            else if (command == "report-util") {
                ConsoleManager::getInstance()->listFinishedProcesses(true); // set WriteToFile as true 
                std::cout << "Report generated at C:/csopesy-log.txt!" << std::endl;
            }
            else {
                std::cerr << command + " invalid command." << std::endl;
            }

        }
        
    }
