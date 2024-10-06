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
    std::cerr << "Process Name: " + this->attachedProcess->getName() << std::endl; 
    std::cerr << "ID: " + std::to_string(this->attachedProcess->getID()) << std::endl; 
    std::cerr << "Current Line of Instruction: "
        << this->attachedProcess->getCurrentLine() << '/'
        << this->attachedProcess->getTotalLines() << std::endl; 

    //(HH/DD/YYYY,HH:MM:SS AM/PM)
    std::cerr << "Created: " + this->attachedProcess->getTimeCreated() << std::endl; 
}

