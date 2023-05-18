#include <iostream>
#include <fstream>
#include <string>

#include "NonDeterministicFiniteAutomaton.h"

int main()
{
	//verificam daca am construit corect automatul citit
	NondeterministicFiniteAutomaton NFA;
	NFA.ReadNFA();
	NFA.PrintAutomaton();
	
	return 0;
}