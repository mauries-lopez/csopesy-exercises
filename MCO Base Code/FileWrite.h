#pragma once
#pragma once
#include <string>

class FileWrite {
public:
    static void generateFile(int processID, const std::string& processName, const std::string& time);
};
