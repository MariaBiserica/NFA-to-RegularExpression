#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

#include "NonDeterministicFiniteAutomaton.h"

/*
TEXT COLORS
1  : Blue
2  : Green
3  : Cyan
4  : Red
5  : Purple
6  : Yellow(dark)
7  : Default white
8  : Grey
9  : Bright blue
10 : Bright green
11 : Bright cyan
12 : Bright red
13 : pink
14 : yellow
15 : Bright white
*/

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Checking if the read automaton is constructed correctly
	NondeterministicFiniteAutomaton NFA;
	if (NFA.ReadNFA())
	{
		NFA.PrintNFA();
		std::cout << std::endl << std::endl;

		// Step 1. Adding a new initial and final state
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << "Step 1. Adding a new initial and final state" << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
		NFA.AddNewInitialState();
		NFA.AddNewFinalState();
		NFA.PrintNFA();
		std::cout << std::endl << std::endl;

		// Step 2. Reducing the states with the fewest transitions until only the initial and final state remain
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << "Step 2. Reducing the states with the fewest transitions until only the initial and final state remain" << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
		while (NFA.GetStates().size() > 2)
		{
			std::string stateToDelete = NFA.getNextStateToDelete();
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "We will eliminate the state: " << stateToDelete << std::endl;
			SetConsoleTextAttribute(hConsole, 7);
			NFA.RemoveState(stateToDelete);
			NFA.PrintNFA();
			std::cout << "--------------------------------------------------------------" << std::endl;
		}

		// Step 3. Displaying the obtained regular expression
		auto automaton = NFA.GetTransitions().GetDeltaFunction();
		auto regularExpression = automaton.begin()->first.second;
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "The obtained regular expression is: ";
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << regularExpression << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}

	return 0;
}