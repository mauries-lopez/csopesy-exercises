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
	long long batchProcessFreq = 0;

	// Other Variables (For Process)
	long long minimumIns = 0;
	long long maximumIns = 0;
	long long delaysPerExec = 0;

private:
	bool refresh;
};