#include "BaseScreen.h"
#include "ConsoleManager.h"
#include <iostream>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName){
    this->attachedProcess = process;
}

void BaseScreen::onEnabled() {
    this->refresh = false;
}

void BaseScreen::process() {

    if (this->refresh == false) {
        printProcessInfo();
        this->refresh = true;
    }

    String command = "";
    std::cout << "@BaseScreen->: ";
    getline(std::cin, command);

    if (command == "exit") {
        ConsoleManager::getInstance()->switchToScreen(MAIN_CONSOLE);
    }
    else {
        std::cerr << "Invalid Command." << std::endl;
    }
}

void BaseScreen::display() {}

void BaseScreen::printProcessInfo() const {
    std::cerr << "Process Name: " + this->attachedProcess->processName << std::endl;
    std::cerr << "ID: " + std::to_string(this->attachedProcess->processID) << std::endl;
    std::cerr << "Current Line of Instruction: " + std::to_string(this->attachedProcess->currLineOfInstruction) + '/' + std::to_string(this->attachedProcess->totalLineOfInstruction) << std::endl;
    
    //(HH/DD/YYYY,HH:MM:SS AM/PM)
    std::cerr << "Created: " + this->attachedProcess->timeCreated << std::endl;
}

