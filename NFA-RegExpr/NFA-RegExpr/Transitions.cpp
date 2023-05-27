#include "Transitions.h"
#include <Windows.h>

void Transitions::PrintTransitions()
{
	for (const auto& element : m_delta)
	{
		if (element.second.size() == 1)
		{
			std::cout << "delta ( " << element.first.first << ", " << element.first.second << " ) = ";
			std::cout << element.second[0] << std::endl;
		}
		else
		{
			std::cout << "delta ( " << element.first.first << ", " << element.first.second << " ) = {";
			for (int i = 0; i < element.second.size(); i++)
			{
				if (i != (element.second.size() - 1))
					std::cout << element.second[i] << ", ";
				else
					std::cout << element.second[i] << "}" << std::endl;
			}
		}
	}
}

void Transitions::InsertTransition(std::string transitionState, std::string transitionSymbol, std::vector<std::string> transitionResultStates)
{
	if (!ExistsState(transitionState))
		m_states.push_back(transitionState);

	for (auto& s : transitionResultStates)
		if (!ExistsState(s))
			m_states.push_back(s);

	if (!ExistsSymbol(transitionSymbol))
		m_symbols += transitionSymbol;

	if (!ExistsTransition(transitionState, transitionSymbol))
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		m_delta.insert({ p, {transitionResultStates} });
	}
	else
	{
		std::pair<std::string, std::string> p = std::make_pair(transitionState, transitionSymbol);
		auto resultState = m_delta.find(p);

		for (auto& s : transitionResultStates)
			if (std::find(resultState->second.begin(), resultState->second.end(), s) == resultState->second.end())
				resultState->second.push_back(s);
	}
}

void Transitions::UpdateTransitionSymbol(TransitionFunctionInputs transitionOldInputs, std::string transitionFinalState, std::string newTransitionSymbol)
{
	// Check if the transition exists
	if (ExistsTransition(transitionOldInputs.first, transitionOldInputs.second))
	{
		// Find the transition in the map
		auto it = m_delta.find(transitionOldInputs);

		if (it != m_delta.end())
		{
			// Find the final state in the transition that corresponds to our transitionFinalState
			for (auto& state : it->second)
			{
				if (state == transitionFinalState)
				{
					// Replace the existing pair in the map with the new pair
					std::string newInputState = it->first.first;
					std::vector<std::string> newOutStates = it->second;

					m_delta.erase(it);

					InsertTransition(newInputState, newTransitionSymbol, newOutStates);
					break;
				}
			}
		}
	}
}

void Transitions::DeleteTransition(TransitionFunctionInputs transitionInputs, std::string transitionFinalState)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Check if the transition exists
	if (ExistsTransition(transitionInputs.first, transitionInputs.second))
	{
		// Find the transition in the map
		auto& transitionOut = m_delta[transitionInputs];

		// Find the index of the final state in the transition that corresponds to our transitionFinalState
		auto it = std::find(transitionOut.begin(), transitionOut.end(), transitionFinalState);
		if (it != transitionOut.end())
		{
			SetConsoleTextAttribute(hConsole, 13);
			std::cout << "Delete: (" << transitionInputs.first << ", " << transitionInputs.second << ") -> " << *it << std::endl;
			SetConsoleTextAttribute(hConsole, 7);

			// Erase the transition result state
			transitionOut.erase(it);

			// If the transition result states vector is empty, remove the entire transition from the map
			if (transitionOut.empty())
				m_delta.erase(transitionInputs);
		}
	}
}

bool Transitions::ExistsEpsilonTransiton()
{
	for (auto& transition : m_delta)
	{
		if (transition.first.second == "E")
			return true;
	}
	return false;
}

bool Transitions::ExistsTransition(std::string transitionState, std::string transitionSymbol)
{
	if (m_delta.find(std::make_pair(transitionState, transitionSymbol)) != m_delta.end())
		return true;
	return false;
}

bool Transitions::ExistsTransitionBetweenStates(std::string inState, std::string outState)
{
	for (auto& transition : m_delta)
	{
		if (transition.first.first == inState && std::find(transition.second.begin(), transition.second.end(), outState) != transition.second.end())
			return true;
	}
	return false;
}

std::vector<std::string> Transitions::GetTransitionsBetweenStates(std::string inState, std::string outState)
{
	//for the given states, return the symbols that have the inState as a start state and the outState as a result state
	std::vector<std::string> transitions;
	for (auto& transition : m_delta)
	{
		if (transition.first.first == inState && std::find(transition.second.begin(), transition.second.end(), outState) != transition.second.end())
			transitions.push_back(transition.first.second);
	}
	return transitions;
}

int Transitions::GetInNumberOfTransitions(std::string state)
{
	//the number of transitions that have the state as a result state
	//dont count the transtions that have the state as a start state

	int inNumberOfTransitions = 0;
	for (const auto& element : m_delta)
	{
		for (const auto& s : element.second)
			if (s == state && element.first.first != state)
				inNumberOfTransitions++;
	}
	return inNumberOfTransitions;
}

int Transitions::GetOutNumberOfTransitions(std::string state)
{
	//the number of transitions that have the state as a start state
	//dont count the transtions that have the state as a result state

	int outNumberOfTransitions = 0;
	for (const auto& element : m_delta)
		if (element.first.first == state)
			for (const auto& s : element.second)
				if (s != state)
					outNumberOfTransitions++;

	return outNumberOfTransitions;
}

std::vector<std::string> Transitions::GetInTransitions(std::string state)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//vector of strings that contains the start states of the transitions that have the state as a result state
	//dont count the transtions that have the state as a start state
	std::vector<std::string> inTransitions;
	for (const auto& element : m_delta)
	{
		for (const auto& s : element.second)
			if (s == state && element.first.first != state)
				inTransitions.push_back(element.first.first);
	}

	//print the result states
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "IN: ";
	for (const auto& s : inTransitions)
		std::cout << s << " ";
	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole, 7);

	return inTransitions;
}

std::vector<std::string> Transitions::GetOutTransitions(std::string state)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//vector of strings that contains the result states of the transitions that have the state as a start state
	//dont count the transtions that have the state as a result state, a state cand have multiple transitions with the same symbol to other states
	std::vector<std::string> outTransitions;
	//	for (const auto& element : m_delta)
	//	{
	////a state cand have with the same symbol multiple transitions to other states
	//		if (element.first.first == state && element.second[0] != state)
	//		{
	//			bool found = false;
	//			for (const auto& s : outTransitions)
	//				if (s == element.second[0])
	//					found = true;
	//			if (!found)
	//				outTransitions.push_back(element.second[0]);
	//		}
	//	}

	//get all the result states of the transitions that have the state as a start state
	for (const auto& element : m_delta)
	{
		//with the same symbol one state can go to itself or to other states. get all of the result states
		if (element.first.first == state)
		{
			for (const auto& s : element.second)
			{
				//get the states that are not the start state
				if (s != state)
				{
					outTransitions.push_back(s);
				}

			}
		}
	}

	//print the start states
	SetConsoleTextAttribute(hConsole, 9);
	std::cout << "OUT:";
	for (const auto& s : outTransitions)
		std::cout << s << " ";
	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole, 7);

	return outTransitions;
}

std::string Transitions::GetTransitionSymbol(std::string state1, std::string state2)
{
	//get the symbol of the transition from state1 to state2
	for (const auto& element : m_delta)
		if (element.first.first == state1)
			for (int i = 0; i < element.second.size(); i++)
				if (element.second[i] == state2)
					return element.first.second;

	return "";
}

Unordered_map Transitions::GetDeltaFunction()
{
	return m_delta;
}


bool Transitions::ExistsState(std::string state)
{
	for (auto& currentState : m_states)
		if (state == currentState)
			return true;
	return false;
}

bool Transitions::ExistsSymbol(std::string symbol)
{
	for (const auto& s : m_symbols)
		if (s == symbol[0])
			return true;
	return false;
}

