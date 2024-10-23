#include <iostream>
#include <string>
#include <sstream> // [Link: https://www.geeksforgeeks.org/processing-strings-using-stdistringstream/]
#include "ConsoleManager.h"

using namespace std; //To not specify the prefix (std::<syntax>)

int main()
{
    ConsoleManager::initialize();
    bool running = true;

    while (running) {
        ConsoleManager::getInstance()->drawConsole();
        ConsoleManager::getInstance()->process();

        running = ConsoleManager::getInstance()->isRunning();
    }
    
    ConsoleManager::destroy();
    return 0;
}