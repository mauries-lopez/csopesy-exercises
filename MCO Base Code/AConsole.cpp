#include <iostream>
#include "TypedefRepo.h"
#include "AConsole.h"
#include "ConsoleManager.h"

AConsole::AConsole(String name) {
	this->name = name;
}

String AConsole::getName() {
	return this->name;
}