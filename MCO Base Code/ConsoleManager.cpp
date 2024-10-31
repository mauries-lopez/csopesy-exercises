#include "ConsoleManager.h"
#include "MainConsole.h"
#include <iostream>
#include "BaseScreen.h"
#include "Process.h"
#include <thread>
#include <fstream>
#include "ScheduleWorker.h"
#include <random>

int ScheduleWorker::usedCores = 0;
int ScheduleWorker::availableCores = 0;
int MainConsole::totalNumCores = 0;

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

bool ConsoleManager::createProcess(const std::string& processName, long long randomInstructions) {
	std::lock_guard<std::mutex> lock(processMutex);

	// Check if process exists in unfinished list
	if (std::any_of(unfinishedProcessList.begin(), unfinishedProcessList.end(),
		[&processName](Process* p) { return p->getName() == processName; })) {
		std::cout << "> Process " << processName << " already exists." << std::endl;
		return false;
	}

	time_t currTime;
	char timeCreation[50];
	struct tm datetime;
	time(&currTime);
	localtime_s(&datetime, &currTime);
	strftime(timeCreation, sizeof(timeCreation), "%m/%d/%Y %I:%M:%S%p", &datetime);
	std::string timeCreated = timeCreation;

	// Create new process
	auto newProcess = std::make_shared<Process>(processName, processID++, randomInstructions, timeCreated);

	// Check for available core and assign
	int assignedCore = scheduler.assignCore();
	if (assignedCore != -1) {
		newProcess->setCoreAssigned(assignedCore);
		std::cout << "Process " << processName << " assigned to core " << assignedCore << std::endl;
	}
	else {
		std::cout << "No Available Core. Process " << processName << " is queued." << std::endl;
	}

	// Add new process to the list, start scheduling
	unfinishedProcessList.push_back(newProcess.get());
	std::thread processThread(&ScheduleWorker::scheduleProcess, &scheduler, newProcess);
	processThread.detach(); // Detach the thread 

	return true;
}


void ConsoleManager::schedulerTest(long long batchProcessFreq, long long minIns, long long maxIns) {
	std::thread([this, batchProcessFreq, minIns, maxIns] {
		schedulerTestRun = true;

		int cpuCycles = 1;
		int processIndex = 1;

		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<long long> dis(minIns, maxIns);

		while (schedulerTestRun) {
			// Create new process at every batch frequency
			if (cpuCycles % batchProcessFreq == 0) {
				std::string processName = "process" + std::to_string(processIndex);

				// Random instruct for minIns and maxIns range
				long long randomInstructions = dis(gen);

				// Call createProcess with the generated process name and instructions
				createProcess(processName, randomInstructions);
				std::cout << "Created " << processName << " with " << randomInstructions << " instructions." << std::endl;

				processIndex++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay the loop for 100ms per cycle, can change

			cpuCycles++;
		}
		}).detach();
}

void ConsoleManager::listFinishedProcesses(bool writeToFile) {
	std::ostream* outStream;
	std::ofstream logFile;

	// Write to file only if report-util is called
	if (writeToFile) {
		logFile.open("csopesy-log.txt", std::ios::out | std::ios::trunc);
		if (!logFile.is_open()) {
			std::cerr << "Error: Unable to open log file." << std::endl;
			return;
		}
		outStream = &logFile; // Point to the file stream
	}
	else {
		outStream = &std::cout; // Point to the console output
	}

	// TODO: call respective variables
	// Compute CPU Utilization
	int cpuUtilPercent = (static_cast<float>(ScheduleWorker::usedCores) / MainConsole::totalNumCores) * 100;
	int availCores = abs((ScheduleWorker::usedCores - ScheduleWorker::availableCores));
	*outStream << "\nCPU utilization: " << cpuUtilPercent << "%/100%" << "\nCores used: " << ScheduleWorker::usedCores << "\nCores available: " << availCores << "\n\n" << std::endl;

	*outStream << "Running Processes:" << std::endl;
	for (const auto& process : unfinishedProcessList) {
		*outStream << process->getName() // Process Name
			<< "\t" << "(" << process->getTimeCreated() << ")" // Timestamp of time created
			<< "\t" << "Core: " << process->getCoreAssigned() // Core that worked on process
			<< "\t" << process->getCurrentLine() << " / " << process->getTotalLines() // Current line / total line of execution
			//<< ", Unfinished: " << (process->isFinished() ? "Yes" : "No") // Unfinished? Y/N
			<< std::endl;
	}

	std::cout << "\n\n";

	*outStream << "\n\nFinished Processes:" << std::endl;
	for (const auto& process : finishedProcesses) {
		*outStream << process->getName() 
			<< "\t" << "(" << process->getTimeCreated() << ")" // Timestamp of time created
			<< "\t" << "Core: " << process->getCoreAssigned() // Core that worked on process
			<< "\t" << process->getCurrentLine() << " / " << process->getTotalLines() // Current line / total line of execution
			<< ", Finished: " << (process->isFinished() ? "Yes" : "No")
			<< std::endl;
	}

	if (writeToFile) {
		logFile.close();
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