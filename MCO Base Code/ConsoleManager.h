#pragma once

#include <memory>
#include "AConsole.h"
#include <unordered_map>
#include <Windows.h>
#include "TypedefRepo.h"
#include "BaseScreen.h"
#include "ScheduleWorker.h" 

const String MAIN_CONSOLE = "MAIN_CONSOLE";

//Singleton Instance (Design Pattern)
// -> A class once its instantiated, its globally accessible. (kahit sinong class pwede i-access). 
// -> Only one instance.
class ConsoleManager
{
public:
	typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;

	// Shared_ptr : If matagal ang lifetime (Anything persistent), gagawin dynamically allocated kaya nasa HEAP memory siya.
	//            : Nag au-automatic deallocate dahil dynamic siya. Kapag nakitang di ginagamit.
	// Ptr : If gusto natin persistent ung ConsoleManager whole lifetime of the application, gawin natin pointer.

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole() const;
	void process() const;
	void switchConsole(String consoleName);

	void registerScreen(std::shared_ptr<BaseScreen> screenRef);
	void switchToScreen(String screenName);
	void nullProcessName();
	void invalidPrefix();
	void unregisterScreen(String screenName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning() const;
	void addFinishedProcess(Process* process);
	std::vector<Process*> unfinishedProcessList;
	std::vector<Process*> finishedProcesses;
	void listFinishedProcesses(bool writeToFile = false);

	HANDLE getConsoleHandle() const;

	void setCursorPosition(int posX, int posY) const;

	void schedulerTest(long long batchProcessFreq, long long minIns, long long maxIns);
	void schedulerTestStop() { schedulerTestRun = false; }
	//void createProcess(const std::string& processName, long long randomInstructions);
	bool createProcess(const std::string& processName, long long randomInstructions);

	static int processID;

private:
	ScheduleWorker scheduler;
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {};
	ConsoleManager& operator=(ConsoleManager const&) {};
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;
	std::mutex processMutex;

	HANDLE consoleHandle;
	bool running = true;
	bool schedulerTestRun = false;
};
