#include "BaseScreen.h"
#include "ConsoleManager.h"
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

    // Thread for handling user input concurrently
    //std::thread baseScreenInputThread(&BaseScreen::baseScreenInput, this);
    this->baseScreenInput();

    // Thread for incrementing the process lines in the background
    std::thread processIncrementLine(&Process::incrementLine, this->attachedProcess);

    if (this->attachedProcess->isFinished() == true) {
        ConsoleManager::getInstance()->addFinishedProcess(this->attachedProcess);
    }
    
    // Wait for both threads to finish
    processIncrementLine.detach();
    //baseScreenInputThread.join();
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
}

void BaseScreen::baseScreenInput() {

    String command = "";
    do {
        
        std::cout << "@BaseScreen->: ";
        getline(std::cin, command);
    } while (command != "exit");

    if (command == "exit") {
        ConsoleManager::getInstance()->switchToScreen(MAIN_CONSOLE);
    }
    else {
        std::cerr << "Invalid Command." << std::endl;
    }
    
}

