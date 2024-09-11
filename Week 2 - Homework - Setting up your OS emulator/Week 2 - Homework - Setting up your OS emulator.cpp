#include <iostream>

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

static string askCommandInput() {
    string command = "";
    cout << "Enter a command: ";
    cin >> command;

    return command;
}

static void command_Initialize(string commandInput) {
    cout << commandInput + " command recognized. Doing something.\n";
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

static void helper_CheckValidCommand(string commandInput) {

    string validCommands[7] = { "initialize", "screen", "scheduler-test", "scheduler-stop", "report-util", "clear", "exit" };
    int validCommand_index = 0;
    bool isValid = false;

    // Check if command is a valid command.
    for (int i = 0; i < 7; i++) {
        if (commandInput == validCommands[i]) {
            isValid = true;
            validCommand_index = i;
        }
    }

    if (isValid == true) {
        // [0] = initialize, [1] = screen, [2] = scheduler-test, [3] = scheduler-stop, [4] = report-util, [5] = clear, [6] = exit
        switch (validCommand_index) {
        case 0:
            command_Initialize(commandInput);
            break;
        case 1:
            command_Screen(commandInput);
            break;
        case 2:
            command_SchedulerTest(commandInput);
            break;
        case 3:
            command_SchedulerStop(commandInput);
            break;
        case 4:
            command_ReportUtil(commandInput);
            break;
        case 5:
            command_Clear(commandInput);
            break;
        case 6:
            command_Exit(commandInput);
            break;
        default:
            break;
        }
    }
    else {
        cout << commandInput + " command unrecognized. \n";
    }

}

int main()
{
    // Print the ASCII Text
    asciiPrint();

    // Ask for a commmand until input is "exit"
    string command = "";
    do {
        command = askCommandInput();

        // Check if "command" is valid
        helper_CheckValidCommand(command);

    } while (command != "exit");
    
}

// Hotkeys:
// For batch comment ( Ctrl K + Ctrl C )

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
