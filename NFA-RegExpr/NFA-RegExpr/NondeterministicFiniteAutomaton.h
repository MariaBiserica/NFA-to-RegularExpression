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
	std::vector<std::string> m_states; //Q - finite non-empty set of states = m_VN
	std::string m_alphabet; //sigma - input symbols / input alphabet = m_VT
	std::string m_initialState; //q0 - initial state = m_S
	std::vector<std::string> m_finalStates; //F - set of final states, included in Q
	Transitions m_transitions; //transition function - input: (state, symbol) -> a state
};
