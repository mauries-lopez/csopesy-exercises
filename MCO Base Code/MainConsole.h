#include <iostream>
#include "TypedefRepo.h"
#include "ConsoleManager.h"

class MainConsole : public AConsole {
public:
	MainConsole(String name);
	~MainConsole();

	// Overriding BaseScreen methods
	void display() override;
	void process() override;
	void onEnabled() override;

	// Other Variables (For Whole System)
	static long long batchProcessFreq;
	static int curClockCycle;
	static int totalNumCores;
	String scheduler;

	// Other Variables (For Process)
	static long long quantumCycles;
	static long long minimumIns;
	static long long maximumIns;
	static long long delaysPerExec;

	static std::vector <std::string> processesNameList;

private:
	bool refresh;
};