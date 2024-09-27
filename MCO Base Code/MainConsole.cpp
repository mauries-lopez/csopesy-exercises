#include <iostream>
#include "TypedefRepo.h"
#include "MainConsole.h"
#include "ConsoleManager.h"
#include <sstream>
#include <ctime>

using namespace std; //To not specify the prefix (std::<syntax>)

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

void MainConsole::onEnabled() {}

void MainConsole::display() {
    asciiPrint();
}

void MainConsole::process() {

    string main_CommandInput = "";
    string validCommands[7] = { "initialize", "screen", "scheduler-test", "scheduler-stop", "report-util", "clear", "exit" };

    //// Initialize a vector to hold active screens
    vector<shared_ptr<BaseScreen>> activeScreens;

    // Ask for user input
    cout << "@MainConsole->: ";
    getline(cin, main_CommandInput);

    // Extract command into tokens
    istringstream stream(main_CommandInput);
    string command, prefix, processName;
    stream >> command; // Read the command, e.g., "screen"

    // Check for specific command formats
    if (command == "screen") {
        stream >> prefix >> processName; // Read the prefix and process name
        if (prefix == "-s" && !processName.empty()) {
            // Assuming a default process ID and capacity for demonstration
            int processID = 1; // This could be generated dynamically

            // Get current time
            time_t currTime;
            char timeCreation[50];
            struct tm datetime;
            time(&currTime);
            localtime_s(&datetime, &currTime);
            strftime(timeCreation, 50, "%m\/%d\/%G, %r", &datetime);

            string timeCreated = (string)timeCreation;

            // Create a new process and associated BaseScreen
            shared_ptr<Process> process = make_shared<Process>(processName, processID, 100, timeCreated);
            shared_ptr<BaseScreen> baseScreen = make_shared<BaseScreen>(process, processName);

            ConsoleManager::getInstance()->registerScreen(baseScreen);
            ConsoleManager::getInstance()->switchConsole(processName);

            // Add the new BaseScreen to the active screens list
            activeScreens.push_back(baseScreen);
        }
        else{
                cout << "Invalid usage. Use: screen -s <process_name>" << endl;
        }
    }
    else if (command == "exit") {
        ConsoleManager::getInstance()->exitApplication();
    }
}

