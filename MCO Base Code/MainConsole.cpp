#include <iostream>
#include "TypedefRepo.h"
#include "MainConsole.h"
#include "ConsoleManager.h"
#include <sstream>
#include <ctime>

using namespace std; //To not specify the prefix (std::<syntax>)

int processID = 0; // This could be generated dynamically

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
            // Get current time
            time_t currTime;
            char timeCreation[50];
            struct tm datetime;
            time(&currTime);
            localtime_s(&datetime, &currTime);
            strftime(timeCreation, 50, "%m/%d/%G, %r", &datetime);

            string timeCreated = (string)timeCreation;

            // Create a new process and associated BaseScreen
            shared_ptr<Process> process = make_shared<Process>(processName, processID++, 100, timeCreated);
            shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);

            bool isScreenAvailable = ConsoleManager::getInstance()->registerScreen(baseScreen);

            if (isScreenAvailable == true) {
                ConsoleManager::getInstance()->switchConsole(processName);

                // Add the new BaseScreen to the active screens list
                activeScreens.push_back(baseScreen);
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

