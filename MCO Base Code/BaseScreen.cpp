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

    //// Add Process to ProcessList
    //ScheduleWorker scheduleWorker;
    //scheduleWorker.addProcess(this->attachedProcess);
    //// If schedulerQueue is free/empty, put the process inside schedulerQueue to be executed. Else, stay in ProcessList
    //if ( !scheduleWorker.schedulerQueue.empty()) {
    //    scheduleWorker.executeProcess();
    //}

    // Thread for handling user input concurrently
    //std::thread baseScreenInputThread(&BaseScreen::baseScreenInput, this); //Input
    this->baseScreenInput();
    
    //// CONTINOUSLY check if the process is already finish or not
    //while (true) {
    //    std::vector<std::shared_ptr<Process>> finishedProcessesList = ConsoleManager::getInstance()->finishedProcesses;
    //    // If Process is finish
    //    if (this->attachedProcess->isFinished()) {

    //        bool finishedProcess = false;
    //        // Check if the process is already in the finishProcesses List
    //        for (int i = 0; i < finishedProcessesList.size(); i++) {
    //            if (finishedProcessesList[i] == this->attachedProcess) {
    //                finishedProcess = true;
    //            }
    //        }

    //        // If process is not yet MARKED as finished, mark it as finished. Else, do nothing.
    //        if (finishedProcess == false) {
    //            ConsoleManager::getInstance()->addFinishedProcess(this->attachedProcess);
    //        }
    //    }
    //}

    
    // Wait for both threads to finish
    //processIncrementLine.detach();
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
    } while (command != "exit");

    if (command == "exit") {
        ConsoleManager::getInstance()->switchToScreen(MAIN_CONSOLE);
    }
    else {
        std::cerr << "Invalid Command." << std::endl;
    }
    
}

