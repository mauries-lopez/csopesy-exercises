#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include "BaseScreen.h"
#include "Process.h"
#include <thread>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance() {
	return sharedInstance;
}

void ConsoleManager::initialize() {
	sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy() {
	delete sharedInstance;
}

void ConsoleManager::drawConsole() const {
	if (this->currentConsole != nullptr) {
		this->currentConsole->display();
	}
	else {
		std::cerr << "There is no assigned console. Please check." << std::endl;
	}
}

void ConsoleManager::process() const {
	if (this->currentConsole != nullptr) {
		this->currentConsole->process();
	}
	else {
		std::cerr << "There is no assigned console. Please check." << std::endl;
	}
}

void ConsoleManager::switchConsole(String consoleName) {
	if (this->consoleTable.contains(consoleName)) {
		// Clear the screen
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else {
		std::cerr << "Console Name " << consoleName << " not found. Was it initialized?" << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef) {
	if (this->consoleTable.contains(screenRef->getName())) {
		std::cerr << "Screen name " << screenRef->getName() << " already exists. Please use a different name." << std::endl;
	}

	this->consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::switchToScreen(String screenName) {
	if (this->consoleTable.contains(screenName)) {
		//Clear the screen
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else {
		std::cerr << "Screen Name " << screenName << " not found. Was it initialized?" << std::endl;
	}
}

void ConsoleManager::nullProcessName() {
	std::cerr << "You did not enter a process name. Enter a Screen Name." << std::endl;
}

void ConsoleManager::invalidPrefix() {
	std::cerr << "Invalid prefix. Use \'-s\' to start a new screen or \'-r\' to reattach an existing screen." << std::endl;
}

void ConsoleManager::unregisterScreen(String screenName) {
	if (this->consoleTable.contains(screenName)) {
		this->consoleTable.erase(screenName);
	}
	else {
		std::cerr << "Unable to unregister " << screenName << ". Was it registered?" << std::endl;
	}
}

void ConsoleManager::returnToPreviousConsole() {
	if (this->previousConsole != nullptr) {
		// Clear the screen
		system("cls");
		this->currentConsole = this->previousConsole;
		this->previousConsole = nullptr;
		this->currentConsole->onEnabled();
	}
	else {
		std::cerr << "Previous Console not found." << std::endl;
	}
}

void ConsoleManager::exitApplication() {
	this->running = false;
}

bool ConsoleManager::isRunning() const {
	return this->running;
}

void ConsoleManager::addFinishedProcess(Process* process) {
	
	for (int i = 0; i < unfinishedProcessList.size(); i++) {
		if (unfinishedProcessList[i] == process) {
			//Remove the process
			unfinishedProcessList.erase(unfinishedProcessList.begin() + i);
		}
	}

	finishedProcesses.push_back(process);
}

void ConsoleManager::listFinishedProcesses() {
	std::cout << "[Running Processes:]" << std::endl;
	for (const auto& process : unfinishedProcessList) {
		std::cout << process->getName() // Process Name
			<< "\t" << "(" << process->getTimeCreated() << ")" // Timestamp of time created
			<< "\t" << "Core: " << process->getCoreAssigned() // Core that worked on process
			<< "\t" << process->getCurrentLine() << " \/ " << process->getTotalLines() // Current line / total line of execution
			//<< ", Unfinished: " << (process->isFinished() ? "Yes" : "No") // Unfinished? Y/N
			<< std::endl;
	}

	std::cout << "\n\n";

	std::cout << "[Finished Processes:]" << std::endl;
	for (const auto& process : finishedProcesses) {
		std::cout << process->getName()
			<< "\t" << "(" << process->getTimeCreated() << ")" // Timestamp of time created
			<< "\t" << "Core: " << process->getCoreAssigned() // Core that worked on process
			<< "\t" << process->getCurrentLine() << " \/ " << process->getTotalLines() // Current line / total line of execution
			<< ", Finished: " << (process->isFinished() ? "Yes" : "No")
			<< std::endl;
	}
}

ConsoleManager::ConsoleManager() {

	this->running = true;

	//Initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>(MAIN_CONSOLE);

	this->consoleTable[MAIN_CONSOLE] = mainConsole;

	this->switchConsole(MAIN_CONSOLE);
}