#include "Process.h"
#include "BaseScreen.h"
#include "ConsoleManager.h"
#include <iostream>
#include <thread>
#include <mutex>


Process::Process(const std::string& name, int id, int totalLines, const std::string& timeCreated)
    : processName(name), processID(id), totalLineOfInstruction(totalLines),
    currLineOfInstruction(0), timeCreated(timeCreated) {}

// Constructor to initialize the process with a name, ID, and other properties
//Process::Process(String name, int id, int totalLineOfInstruction, String timeCreated) {
//	this->processName = name;
//	this->processID = id;
//	this->currLineOfInstruction = 0;
//	this->totalLineOfInstruction = totalLineOfInstruction;
//	this->timeCreated = timeCreated;
//}

void Process::incrementLine() {
    for (int i = this->currLineOfInstruction; i <= this->totalLineOfInstruction; i++) {
        //std::cout << "Hello World from" << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        this->currLineOfInstruction = i;
        std::this_thread::sleep_for(std::chrono::seconds(0));
    }
}