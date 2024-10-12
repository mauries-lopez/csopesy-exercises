#pragma once
#pragma once
#include <string>
#include <vector>

class FileWrite {
public:
    static void generateFile(int processID, const std::string& processName, const std::string& timeCreated, const std::vector<std::string>& logs);
};

