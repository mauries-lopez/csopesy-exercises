#include <iostream>
#include "TypedefRepo.h"
#include "MainConsole.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include "AConsole.h"
#include <sstream>
#include <ctime>
#include <thread>

using namespace std; //To not specify the prefix (std::<syntax>)

namespace fs = std::filesystem;

int processID = 0; // This could be generated dynamically
std::vector <std::string> processesNameList;

// Waiting queue

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
            shared_ptr<Process> process = make_shared<Process>(processName, i, 100, timeCreated);

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
    

    // Create 10 processes, each with 100 print commands, upon the start of your OS emulator.
 /*   shared_ptr<Process> process1 = make_shared<Process>("process1", 0, 100, "10/11/2024, 09:16:09 PM");
    shared_ptr<Process> process2 = make_shared<Process>("process2", 1, 100, "10/11/2024, 09:17:09 PM");
    shared_ptr<Process> process3 = make_shared<Process>("process3", 2, 100, "10/11/2024, 09:18:09 PM");
    shared_ptr<Process> process4 = make_shared<Process>("process4", 3, 100, "10/11/2024, 09:19:09 PM");
    shared_ptr<Process> process5 = make_shared<Process>("process5", 4, 100, "10/11/2024, 09:20:09 PM");
    shared_ptr<Process> process6 = make_shared<Process>("process6", 5, 100, "10/11/2024, 09:21:09 PM");
    shared_ptr<Process> process7 = make_shared<Process>("process7", 6, 100, "10/11/2024, 09:22:09 PM");
    shared_ptr<Process> process8 = make_shared<Process>("process8", 7, 100, "10/11/2024, 09:23:09 PM");
    shared_ptr<Process> process9 = make_shared<Process>("process9", 8, 100, "10/11/2024, 09:24:09 PM");
    shared_ptr<Process> process10 = make_shared<Process>("process10", 9, 100, "10/11/2024, 09:25:09 PM");*/

    /*ScheduleWorker::addProcess(process1);
    ScheduleWorker::addProcess(process2);
    ScheduleWorker::addProcess(process3);
    ScheduleWorker::addProcess(process4);
    ScheduleWorker::addWaitProcess(process5);
    ScheduleWorker::addWaitProcess(process6);
    ScheduleWorker::addWaitProcess(process7);
    ScheduleWorker::addWaitProcess(process8);
    ScheduleWorker::addWaitProcess(process9);
    ScheduleWorker::addWaitProcess(process10);*/

    /*ConsoleManager::ConsoleTable consoleTable;
    consoleTable["process1"] = std::make_shared<AConsole>("process1");*/

    // Manually adding to processesNameList vector (currently unable to enter BaseScreen using screen -r) -> need to register process name to consoleTable
    /*processesNameList.push_back("process1");
    processesNameList.push_back("process2");
    processesNameList.push_back("process3");
    processesNameList.push_back("process4");
    processesNameList.push_back("process5");
    processesNameList.push_back("process6");
    processesNameList.push_back("process7");
    processesNameList.push_back("process8");
    processesNameList.push_back("process9");
    processesNameList.push_back("process10");*/

    string main_CommandInput = "";
    //string validCommands[7] = { "initialize", "screen", "scheduler-test", "scheduler-stop", "report-util", "clear", "exit" };

    //// Initialize a vector to hold active screens
    vector<shared_ptr<BaseScreen>> activeScreens;

    // Ask for user input
    cout << "@MainConsole->: ";
    getline(cin, main_CommandInput);

    // Extract command into tokens
    istringstream stream(main_CommandInput);
    string command, prefix, processName;
    stream >> command >> prefix >> processName;

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

            //// addProcess if there is available core
            //if (anyAvailableCore) {
            //    ScheduleWorker::addProcess(process);
            //}
            //else { // add to waiting queue if no available core
            //    ScheduleWorker::addWaitProcess(process);
            //}

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
                } else {
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
    else if (command == "exit") {
        ConsoleManager::getInstance()->exitApplication();
    }
}

