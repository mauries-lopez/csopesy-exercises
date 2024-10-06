#include "Process.h"

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