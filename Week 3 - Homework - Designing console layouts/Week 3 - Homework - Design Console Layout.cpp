#include <iostream>
#include <string>
#include <sstream> // [Link: https://www.geeksforgeeks.org/processing-strings-using-stdistringstream/]

using namespace std; //To not specify the prefix (std::<syntax>)

static void asciiPrint() {
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

static string askInitializeInput() {
    string command = "";

    // Application will not start unless user types "initialize"
    do {
        cout << "Enter a command: ";
        getline(cin, command);

        if (command != "initialize") {
            cout << command + " not recognized. Initialize first.\n";
        };

    } while (command != "initialize");

    if ( command == "initialize") {
        cout << "Initialized Complete... \n";
        return command;
    }
}

static void command_Initialize(string commandInput) {
    cout << commandInput + " command recognized. Application already been initialized.\n";
}

static void command_Screen(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
}

static void command_SchedulerTest(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
}

static void command_SchedulerStop(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
}

static void command_ReportUtil(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
}

static void command_Clear(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
    if (system("clear"))
        system("cls");

    asciiPrint();
}

static void command_Exit(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
}

static void startMainApp() {

    string main_CommandInput = "";
    string validCommands[7] = { "initialize", "screen", "scheduler-test", "scheduler-stop", "report-util", "clear", "exit" };

    // Main Window Loop. Exit only if command is "exit"
    do {

        // Ask for user input
        cout << "Enter a command: ";
        getline(cin, main_CommandInput);

        // Extract command into 3 tokens. Ex: screen -s process_1. -> | screen |   -s    | proces_1 |
        //                                                             (prefix) (command) (processName)
        istringstream stream(main_CommandInput);
        string prefix, command, processName;
        stream >> prefix >> command >> processName;

        int validCommand_index = 0; 
        bool isValid = false;

        // Check if command is a valid command.
        for (int i = 0; i < 7; i++) {
            if (main_CommandInput == validCommands[i]) {
                isValid = true;
                validCommand_index = i;
            }
        }

        if (isValid == true) {
            // [1] = screen, [2] = scheduler-test, [3] = scheduler-stop, [4] = report-util, [5] = clear, [6] = exit
            switch (validCommand_index) {
            case 0:
                command_Initialize(main_CommandInput);
            case 1:
                command_Screen(main_CommandInput);
                break;
            case 2:
                command_SchedulerTest(main_CommandInput);
                break;
            case 3:
                command_SchedulerStop(main_CommandInput);
                break;
            case 4:
                command_ReportUtil(main_CommandInput);
                break;
            case 5:
                command_Clear(main_CommandInput);
                break;
            case 6:
                command_Exit(main_CommandInput);
                break;
            default:
                break;
            }
        }
        else {
            cout << main_CommandInput + " command unrecognized. \n";
        }
    } while (main_CommandInput != "exit");
       
}

int main()
{
    // Print the ASCII Text
    asciiPrint();

    // Ask for initialization command
    string command = "";
    command = askInitializeInput();

    // Check if application is initialized. Proceed if initialized.
    if (command == "initialize") {
        // Start Main Window
        startMainApp();
    }
    
}

// Hotkeys:
// For batch comment ( Ctrl K + Ctrl C )
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Notes:
// getLine() <-- Function from string library. Unlike cin, reads string including whitespace. 
