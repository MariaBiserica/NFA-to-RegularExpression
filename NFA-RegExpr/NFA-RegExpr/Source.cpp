#include <iostream>
#include <fstream>
#include <string>

#include "NonDeterministicFiniteAutomaton.h"

int main()
{
	// Verificam daca am construit corect automatul citit
	NondeterministicFiniteAutomaton NFA;
	NFA.ReadNFA();
	NFA.PrintNFA();
	std::cout << std::endl << std::endl;

	// Pasul 1. Adaugam o noua stare initiala si finala
	std::cout << "Pasul 1. Adaugam o noua stare initiala si finala" << std::endl;
	NFA.AddNewInitialState();
	NFA.AddNewFinalState();
	NFA.PrintNFA();
	std::cout << std::endl << std::endl;
	
	// Pasul 2. Reducem starile care au cele mai putine tranzitii pana cand raman doar starea initiala si cea finala
	std::cout << "Pasul 2. Eliminam starile inutile" << std::endl;
	while (NFA.GetStates().size() > 2)
	{
		std::string stateToDelete = NFA.getNextStateToDelete();
		std::cout << "Vom elimina starea: " << stateToDelete << std::endl;
		NFA.RemoveState(stateToDelete);
		NFA.PrintNFA();
		std::cout << "--------------------------------------------------------------" << std::endl;
	}
	
	// Pasul 3. Afisam expresia regulata obtinuta
	auto automaton = NFA.GetTransitions().GetDeltaFunction();
	auto regularExpression = automaton.begin()->first.second;
	std::cout << "Expresia regulata obtinuta este: " << regularExpression << std::endl;
	
	return 0;
}