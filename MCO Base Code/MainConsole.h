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
};