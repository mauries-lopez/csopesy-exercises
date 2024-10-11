#include "FileWrite.h"
#include <fstream>
#include <iostream>
#include <string>

void FileWrite::generateFile(int processID, const std::string& processName, const std::string& timeCreated) {
    std::string fileName = "process_" + processName + ".txt";
    std::string content = "Process name: " + processName + "\n" +
        "Logs:\n" +
        "(" + timeCreated + ") Core: " + std::to_string(processID) +
        " Hello world from screen_" + (processName) + "!";

    std::ofstream outFile(fileName);
    if (outFile) {
        outFile << content; 
        outFile.close(); 
    }
    else {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
    }
}
