#pragma once
#include "Transitions.h"

#include <string>

class NondeterministicFiniteAutomaton
{
public:
	NondeterministicFiniteAutomaton();
	NondeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates);
	
	void ReadNFA();
	void PrintNFA();
	
	//create a new initial state
	void AddNewInitialState();
	//create a new final state
	void AddNewFinalState();
	//get the first state with the lowest number of transitions
	std::string getNextStateToDelete();

	//remove a state and rewite the transitions
	void RemoveState(std::string stateToErase);

	Transitions GetTransitions();
	std::vector<std::string> GetStates();
	std::string GetInitialState();
	std::vector<std::string> GetFinalStates();
	std::string GetAlphabet();

private:
	std::vector<std::string> m_states; //Q - multime finita nevida de stari = m_VN
	std::string m_alphabet; //sigma - simbolurile / alfabetul de intrare = m_VT
	std::string m_initialState; //q0 - stare initiala = m_S
	std::vector<std::string> m_finalStates; //F - multimea starilor finale, inclusa in Q
	Transitions m_transitions; //transition function - input:(stare, simbol) -> o stare
};
