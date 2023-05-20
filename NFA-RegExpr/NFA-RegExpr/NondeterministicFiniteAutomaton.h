#pragma once
#include "Transitions.h"

#include <string>

class NondeterministicFiniteAutomaton
{
public:
	bool ReadNFA();
	void PrintNFA();
	
public:
	void AddNewInitialState();
	void AddNewFinalState();
	
	//get the first state with the lowest number of transitions
	std::string getNextStateToDelete();

	//remove a state and rewrite the transitions
	void RemoveState(std::string stateToErase);

public:
	Transitions GetTransitions();
	std::vector<std::string> GetStates();

private:
	std::vector<std::string> m_states; //Q - multime finita nevida de stari = m_VN
	std::string m_alphabet; //sigma - simbolurile / alfabetul de intrare = m_VT
	std::string m_initialState; //q0 - stare initiala = m_S
	std::vector<std::string> m_finalStates; //F - multimea starilor finale, inclusa in Q
	Transitions m_transitions; //transition function - input:(stare, simbol) -> o stare
};
