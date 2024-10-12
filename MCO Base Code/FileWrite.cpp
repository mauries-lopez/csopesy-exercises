#include "FileWrite.h"
#include <vector>  // Include this
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void FileWrite::generateFile(int processID, const std::string& processName, const std::string& timeCreated, const std::vector<std::string>& logs) {
    // Specify the folder path where you want to save the file
    std::string folderPath = fs::current_path().string();  // Change this to the desired location

    // Concatenate the folder path and the file name
    std::string fileName = folderPath + "\\process_" + std::to_string(processID) + "_" + processName + ".txt";

    std::ofstream outFile(fileName, std::ios::out | std::ios::app);
    if (outFile) {
        outFile << "Process Name: " << processName << "\n";
        outFile << "Process ID: " << processID << "\n";
        outFile << "Created: " << timeCreated << "\n\n";
        outFile << "Logs:\n";

        for (const auto& log : logs) {
            outFile << log << "\n";
        }

        outFile.close();
        std::cerr << "File saved to: " << fileName << std::endl;  // Inform where the file is saved
    }
    else {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
    }
}