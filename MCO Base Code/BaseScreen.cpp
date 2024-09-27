#include "BaseScreen.h"
#include "ConsoleManager.h"
#include <iostream>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName){
    this->attachedProcess = process;
}

void BaseScreen::onEnabled() {
    this->refresh = true;
}

void BaseScreen::process() {
    if (this->refresh == true) {
        String command = "";
        std::cout << "@BaseScreen->: ";
        getline(std::cin, command);

        if (command == "exit") {
            ConsoleManager::getInstance()->switchToScreen(MAIN_CONSOLE);
        }
        else {
            //Fix this in the future
            //This will clear the screen for all invalid commands.
            system("cls");
        }
    }
    else {
        std::cerr << "Not enabled. Process Name: " + this->attachedProcess->processName << std::endl;
    }

}

void BaseScreen::display() {
    printProcessInfo();
}

void BaseScreen::printProcessInfo() const {
    std::cerr << "Process Name: " + this->attachedProcess->processName << std::endl;
    std::cerr << "Current Line of Instruction: " + std::to_string(this->attachedProcess->currLineOfInstruction) + '/' + std::to_string(this->attachedProcess->totalLineOfInstruction) << std::endl;
    
    //(HH/DD/YYYY,HH:MM:SS AM/PM)
    std::cerr << "Created: " + this->attachedProcess->timeCreated << std::endl;
}

