#include "Process.h"

// Constructor to initialize the process with a name, ID, and other properties
Process::Process(String name, int id, int totalLineOfInstruction) {
	this->processName = name;
	this->processID = id;
	this->currLineOfInstruction = 0;
	this->totalLineOfInstruction = totalLineOfInstruction;
}