#pragma once
#include "TypedefRepo.h"

class AConsole
{
public:
	typedef std::string String;
	AConsole(String name);
	~AConsole() = default;

	String getName();
	virtual void onEnabled() = 0; //Tinatawag kapag pinakita ung screen for the first time
	virtual void display() = 0; //Ito ung drawing proper
	virtual void process() = 0; //If there are any input commands.. If need ng certain processing or algorithm na need.. Dito ilalagay.. Computation related

	String name;
	friend class ConsoleManager;

	//friend class?
	// -> ung AConsole class, if need ng functions/variables pwede gamitin niya mula sa ConsoleManager. Regardless if its public, protected, or private.
	// -> Tightly linked naman si AConsole tsaka kay ConsoleManager kaya pwede natin gawin to. ConsoleMaanger is the god class of AConsole.
};
