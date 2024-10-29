#include "BaseScreen.h"
#include "ConsoleManager.h"
#include "ScheduleWorker.h"
#include <iostream>
#include <thread>

BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName){
    this->attachedProcess = process;
}

void BaseScreen::onEnabled() {
    this->refresh = false;
}

void BaseScreen::process() {
    if (this->refresh == false) {
        this->refresh = true;
    }
    this->baseScreenInput();
}


void BaseScreen::display() {
    printProcessInfo();
}

void BaseScreen::printProcessInfo() const {
    std::cerr << "Process Name: " + this->attachedProcess->getName() << std::endl; 
    std::cerr << "ID: " + std::to_string(this->attachedProcess->getID()) << std::endl; 
    std::cerr << "Current Line of Instruction: "
        << this->attachedProcess->getCurrentLine() << '/'
        << this->attachedProcess->getTotalLines() << std::endl;
    //(HH/DD/YYYY,HH:MM:SS AM/PM)
    std::cerr << "Created: " + this->attachedProcess->getTimeCreated() << std::endl; 
    //Append the logs into one array to consitently display it.
    std::vector<std::string> storePrintLogs = this->attachedProcess->getPrintLogs();
    for (int i = 1; i < storePrintLogs.size(); i++) {
        std::cerr << storePrintLogs[i] << std::endl;
    }
}

void BaseScreen::baseScreenInput() {
    String command = "";
    do {
        
        std::cout << "@BaseScreen->: ";
        getline(std::cin, command);

        if (command == "process-smi") {
            if (this->attachedProcess->isFinished() == true) {
                std::cerr << "\nProcess: " << this->attachedProcess->getName() << std::endl;
                std::cerr << "ID: " << std::to_string(this->attachedProcess->getID()) << std::endl;
                std::cerr << "\nFinished!" << std::endl << std::endl;
            }
            else {
                std::cerr << "\nCurrent Line of Instruction: " << this->attachedProcess->getCurrentLine() << std::endl;;
                std::cerr << "Lines of code: " << this->attachedProcess->getTotalLines() << std::endl << std::endl;
            }
        }
        else {
            std::cerr << "Invalid Command." << std::endl;
        }

    } while (command != "exit");

    if (command == "exit") {
        ConsoleManager::getInstance()->switchToScreen(MAIN_CONSOLE);
    }
    else {
        std::cerr << "Invalid Command." << std::endl;
    }
    
}

