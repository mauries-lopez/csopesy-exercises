#pragma once

#include <memory>
#include "AConsole.h"
#include <unordered_map>
#include <Windows.h>
#include "TypedefRepo.h"
#include "BaseScreen.h"

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

	bool registerScreen(std::shared_ptr<BaseScreen> screenRef);
	void switchToScreen(String screenName);
	void nullProcessName();
	void invalidPrefix();
	void unregisterScreen(String screenName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning() const;
	void addFinishedProcess(std::shared_ptr<Process> process);
	void listFinishedProcesses();

	HANDLE getConsoleHandle() const;

	void setCursorPosition(int posX, int posY) const;

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {};
	ConsoleManager& operator=(ConsoleManager const&) {};
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;
	std::vector<std::shared_ptr<Process>> finishedProcesses; 

	HANDLE consoleHandle;
	bool running = true;
};
