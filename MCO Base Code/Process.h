#pragma once
#include "TypedefRepo.h"

class Process {
public:
    // Constructor to initialize the process with a name, ID, and other properties
    Process(String name, int id, int totalLineOfInstruction, String timeCreated);

    // Member variables
    String processName;
    int processID;
    int currLineOfInstruction;
    int totalLineOfInstruction;
    String timeCreated;
};