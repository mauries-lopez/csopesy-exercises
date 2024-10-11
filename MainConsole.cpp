#include <iostream>
#include "TypedefRepo.h"
#include "MainConsole.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include "FileWriter.h"
#include <sstream>
#include <ctime>
#include <thread>

using namespace std; //To not specify the prefix (std::<syntax>)

int processID = 0; // This could be generated dynamically
std::vector <std::string> processesNameList;

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

    // Create 10 processes, each with 100 print commands, upon the start of your OS emulator.


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

    // Extract processName if it exists
    //if (stream) {
    //    getline(stream, processName); // Capture the remaining string as processName
    //    //processName = processName.substr(1); // Remove leading space
    //}
    // ^^^ Line 60 to 63 was commented out on 08-10-2024 due to it making the process name incomplete (e.g. missing first letter). Better to implement removing spaces in the middle of process name

    // Check for specific command formats
    if (command == "screen") {

        if (prefix == "-s" && !processName.empty()) {
            // Get current time
            time_t currTime;
            char timeCreation[50];
            struct tm datetime;
            time(&currTime);
            localtime_s(&datetime, &currTime);
            strftime(timeCreation, 50, "%m/%d/%G, %r", &datetime);

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

                // Write to file
                FileWriter::generateFile(process->getID(), process->getName());
            }
            else {
                std::cerr << "Screen name " << processName << " already exists. Please use a different name." << std::endl;
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
