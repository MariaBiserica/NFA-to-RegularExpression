#include "NondeterministicFiniteAutomaton.h"
#include <Windows.h>
#include<fstream>

bool NondeterministicFiniteAutomaton::ReadNFA()
{
	std::ifstream fin("input11.txt");
	if (!fin.is_open())
	{
		std::cout << "Error opening file!" << std::endl;
		return false;
	}
	else
	{
		//read each state splitted by comma
		std::string states;
		std::getline(fin, states);
		std::string delimiter = ",";
		size_t pos = 0;
		std::string token;
		while ((pos = states.find(delimiter)) != std::string::npos)
		{
			token = states.substr(0, pos);
			m_states.push_back(token);
			states.erase(0, pos + delimiter.length());
		}
		m_states.push_back(states);

		//read alphabet
		std::getline(fin, m_alphabet);
		//read initial state
		std::getline(fin, m_initialState);

		//read final states one by one
		std::string finalStates;
		std::getline(fin, finalStates);
		delimiter = ",";
		pos = 0;
		while ((pos = finalStates.find(delimiter)) != std::string::npos)
		{
			token = finalStates.substr(0, pos);
			m_finalStates.push_back(token);
			finalStates.erase(0, pos + delimiter.length());
		}
		m_finalStates.push_back(finalStates);

		//read transitions
		std::string line;
		while (std::getline(fin, line))
		{

			std::string delimiter = "-";
			size_t pos = 0;
			std::vector<std::string> transition;
			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				token = line.substr(0, pos);
				transition.push_back(token);
				line.erase(0, pos + delimiter.length());
			}
			transition.push_back(line);

			std::string transitionState = transition[0];

			std::string transitionLabel = transition[1];

			// check if the transition label contains ',' and replace it with '+'
			pos = transitionLabel.find(',');
			if (pos != std::string::npos)
			{
				while (pos != std::string::npos) {
					transitionLabel[pos] = '+';
					// find the next occurrence of the old character
					pos = transitionLabel.find(',', pos + 1);
				}
				//transitionLabel = "(" + transitionLabel + ")";
			}

			std::vector<std::string> transitionResult;
			transitionResult.push_back(transition[2]);
			m_transitions.InsertTransition(transitionState, transitionLabel, transitionResult);
		}

	}
	fin.close();
	return true;
}

void NondeterministicFiniteAutomaton::PrintNFA()
{
	std::cout << "Elements of the automaton are: " << std::endl;
	std::cout << "States - Q:{";
	for (int i = 0; i < m_states.size(); i++)
	{
		if (i != (m_states.size() - 1))
			std::cout << m_states[i] << ", ";
		else
			std::cout << m_states[i] << "}" << std::endl;
	}

	std::cout << "Input alphabet - Sigma:{";
	for (int i = 0; i < m_alphabet.size(); i++)
	{
		if (i != (m_alphabet.size() - 1))
			std::cout << m_alphabet[i] << ", ";
		else
			std::cout << m_alphabet[i] << "}" << std::endl;
	}

	std::cout << "Initial state: " << m_initialState << std::endl;

	std::cout << "Set of final states - F:{";
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		if (i != (m_finalStates.size() - 1))
			std::cout << m_finalStates[i] << ", ";
		else
			std::cout << m_finalStates[i] << "}" << std::endl;
	}

	std::cout << "Transition function:" << std::endl;
	m_transitions.PrintTransitions();
}

void NondeterministicFiniteAutomaton::AddNewInitialState()
{
	//create a new initial state and add a new transition from it to the old initial state, update the old initial state
	std::string newInitialState = "S";
	m_states.push_back(newInitialState);

	//add a new transition from the new initial state to the old initial state
	std::vector<std::string> trnsitionResult;
	trnsitionResult.push_back(m_initialState);
	m_transitions.InsertTransition(newInitialState, "E", trnsitionResult);

	//check if 'E' exists in the alphabet, if not add it
	if (m_alphabet.find('E') == std::string::npos)
	{
		m_alphabet += "E";
	}

	//delete the old initial state
	m_initialState = newInitialState;
}

void NondeterministicFiniteAutomaton::AddNewFinalState()
{
	//create a new final state and add a new transition from all the old final states to the new final state, update the old final states
	std::string newFinalState = "F";
	m_states.push_back(newFinalState);

	//add a new transition from all the old final states to the new final state
	for (int i = 0; i < m_finalStates.size(); i++)
	{
		std::vector<std::string> trnsitionResult;
		trnsitionResult.push_back(newFinalState);
		m_transitions.InsertTransition(m_finalStates[i], "E", trnsitionResult);
	}

	//check if 'E' exists in the alphabet, if not add it
	if (m_alphabet.find('E') == std::string::npos)
	{
		m_alphabet += "E";
	}

	//delete the old final states
	m_finalStates.clear();
	m_finalStates.push_back(newFinalState);
}

std::string NondeterministicFiniteAutomaton::getNextStateToDelete()
{
	int minFactor = INT_MAX;
	std::string stateToDelete;
	for (int i = 0; i < m_states.size(); i++)
	{
		if (m_states[i] != m_initialState && m_states[i] != m_finalStates[0])
		{
			//get the number of transitions that have the state as a first state
			int inTransitions = m_transitions.GetInNumberOfTransitions(m_states[i]);
			int outTransitions = m_transitions.GetOutNumberOfTransitions(m_states[i]);
			int factor = inTransitions * outTransitions;
			std::cout << "state: " << m_states[i] << ", factor: " << factor << "-> IN:" << inTransitions << ", OUT:" << outTransitions << std::endl;
			if (factor < minFactor)
			{
				minFactor = factor;
				stateToDelete = m_states[i];
			}
		}
	}
	return stateToDelete;
}

void NondeterministicFiniteAutomaton::RemoveState(std::string stateToErase)
{
	std::vector<std::string> inTransitions = m_transitions.GetInTransitions(stateToErase);
	std::vector<std::string> outTransitions = m_transitions.GetOutTransitions(stateToErase);

	for (int i = 0; i < inTransitions.size(); i++)
	{
		for (int o = 0; o < outTransitions.size(); o++)
		{
			std::vector<std::string> INSymbolsTwoLetters;
			//get all the transitions from the IN state to the state to be deleted
			INSymbolsTwoLetters = m_transitions.GetTransitionsBetweenStates(inTransitions[i], stateToErase);
			//concatenate the symbols of the transitions from the IN state to the state to be deleted
			std::string inSymbol = "";
			if (INSymbolsTwoLetters.size() > 1)
				inSymbol = "(" + INSymbolsTwoLetters[0] + "+" + INSymbolsTwoLetters[1] + ")";
			else
				inSymbol = INSymbolsTwoLetters[0];

			
			//get all the transitions from the state to be deleted to the OUT state
			std::vector<std::string> OUTSymbolsTwoLetters;
			OUTSymbolsTwoLetters = m_transitions.GetTransitionsBetweenStates(stateToErase, outTransitions[o]);
			//concatenate the symbols of the transitions from the state to be deleted to the OUT state
			std::string outSymbol = "";
			if (OUTSymbolsTwoLetters.size() > 1)
				outSymbol = "(" + OUTSymbolsTwoLetters[0] + "+" + OUTSymbolsTwoLetters[1] + ")";
			else
				outSymbol = OUTSymbolsTwoLetters[0];

			
			std::string newTransitionSymbol = "";

			//check if there is a direct transition from the IN state to the OUT state
			if (m_transitions.ExistsTransitionBetweenStates(inTransitions[i], outTransitions[o]))
			{
				//if there is, we eliminate the transition through the state to be deleted and update the direct transition
				//we will add to the symbol of the direct transition: "+" and "the symbol of the intermediate transition through the state to be deleted"
				//we want to concatenate the symbols of the "transitions through the state to be deleted"

				//if there are more direct transitions between the IN state and the OUT state, we will concatenate the symbols of the intermediate transitions
				//through the state to be deleted
				std::vector<std::string> directTransitions = m_transitions.GetTransitionsBetweenStates(inTransitions[i], outTransitions[o]);

				//get the symbol of the direct transition by combining the symbols in the directTransitions vector (if there are more direct transitions)
				std::string directSymbol = "";
				if (directTransitions.size() > 1)
					directSymbol = "(" + directTransitions[0] + directTransitions[1] + ")";
				else if (directTransitions.size() != 0)
					directSymbol = directTransitions[0];

				//for the symbol through the deleted state we can have loop to the deleted state, it must be checked and concatenated in case of affirmative
				std::string loopSymbol = "";
				loopSymbol = m_transitions.GetTransitionSymbol(stateToErase, stateToErase);

				//check if the loop symbol is not empty
				if (loopSymbol != "")
				{
					//let's check the symbols of the transitions
					if (loopSymbol.size() > 1)
					{
						if (directSymbol != "E")
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*(" + outSymbol + ")+" + directSymbol;
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*" + outSymbol + "+(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*" + outSymbol + "+" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*(" + outSymbol + ")+" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*" + outSymbol + "+(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*" + outSymbol + "+" + directSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*+(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*+" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*+(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*+" + directSymbol;
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*(" + outSymbol + ")+" + directSymbol;
								else if (!NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*" + outSymbol + "+(" + directSymbol + ")";
								else if (!NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*" + outSymbol + "+" + directSymbol;
							}
							else if (inSymbol == "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*+(" + directSymbol + ")";
								else
									newTransitionSymbol = "(" + loopSymbol + ")*+" + directSymbol;
							}
						}
						else // direct symbol is "E" or null
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*(" + outSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*" + outSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*(" + outSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*" + outSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*";
								else
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*";
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*(" + outSymbol + ")";
								else
									newTransitionSymbol = "(" + loopSymbol + ")*" + outSymbol;
							}
							else if (inSymbol == "E" && outSymbol == "E")
							{
								newTransitionSymbol = "(" + loopSymbol + ")*";
							}
						}
					}
					else if (loopSymbol.size() == 1)
					{
						if (directSymbol != "E")
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*(" + outSymbol + ")+" + directSymbol;
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*" + outSymbol + "+(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*" + outSymbol + "+" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*(" + outSymbol + ")+" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*" + outSymbol + "+(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*" + outSymbol + "+" + directSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*(" + directSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*" + directSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*(" + directSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*" + directSymbol;
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = loopSymbol + "*(" + outSymbol + ")+(" + directSymbol + ")";
								else if (NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = loopSymbol + "*(" + outSymbol + ")+" + directSymbol;
								else if (!NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
									newTransitionSymbol = loopSymbol + "*" + outSymbol + "+(" + directSymbol + ")";
								else if (!NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
									newTransitionSymbol = loopSymbol + "*" + outSymbol + "+" + directSymbol;
							}
							else if (inSymbol == "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(directSymbol))
									newTransitionSymbol = loopSymbol + "+(" + directSymbol + ")";
								else
									newTransitionSymbol = loopSymbol + "+" + directSymbol;
							}
						}
						else //direct symbol is "E" or null
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*(" + outSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*" + outSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*(" + outSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*" + outSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*";
								else
									newTransitionSymbol = inSymbol + loopSymbol + "*";
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol))
									newTransitionSymbol = loopSymbol + "*(" + outSymbol + ")";
								else
									newTransitionSymbol = loopSymbol + "*" + outSymbol;
							}
							else if (inSymbol == "E" && outSymbol == "E")
							{
								newTransitionSymbol = loopSymbol + "*";
							}
						}
					}
				}
				else //loop symbol is "E" or null
				{
					if (directSymbol != "E")
					{
						if (inSymbol != "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")(" + outSymbol + ")+(" + directSymbol + ")";
							else if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")(" + outSymbol + ")+" + directSymbol;
							else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")" + outSymbol + "+(" + directSymbol + ")";
							else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")" + outSymbol + "+" + directSymbol;
							else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + "(" + outSymbol + ")+(" + directSymbol + ")";
							else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + "(" + outSymbol + ")+" + directSymbol;
							else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + outSymbol + "+(" + directSymbol + ")";
							else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + outSymbol + "+" + directSymbol;
						}
						else if (inSymbol != "E" && outSymbol == "E")
						{
							if (NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")+(" + directSymbol + ")";
							else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + inSymbol + ")+" + directSymbol;
							else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + "+(" + directSymbol + ")";
							else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = inSymbol + "+" + directSymbol;
						}
						else if (inSymbol == "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + outSymbol + ")+(" + directSymbol + ")";
							else if (NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + outSymbol + ")+" + directSymbol;
							else if (!NeedsParenthesis(outSymbol) && NeedsParenthesis(directSymbol))
								newTransitionSymbol = outSymbol + "+(" + directSymbol + ")";
							else if (!NeedsParenthesis(outSymbol) && !NeedsParenthesis(directSymbol))
								newTransitionSymbol = outSymbol + "+" + directSymbol;
						}
						else if (inSymbol == "E" && outSymbol == "E")
						{
							if (NeedsParenthesis(directSymbol))
								newTransitionSymbol = "(" + directSymbol + ")";
							else
								newTransitionSymbol = directSymbol;
						}
					}
					else //direct symbol is "E" or null
					{
						if (inSymbol != "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + inSymbol + ")(" + outSymbol + ")";
							else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + inSymbol + ")" + outSymbol;
							else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
								newTransitionSymbol = inSymbol + "(" + outSymbol + ")";
							else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
								newTransitionSymbol = inSymbol + outSymbol;
						}
						else if (inSymbol != "E" && outSymbol == "E")
						{
							if (NeedsParenthesis(inSymbol))
								newTransitionSymbol = "(" + inSymbol + ")";
							else
								newTransitionSymbol = inSymbol;
						}
						else if (inSymbol == "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + outSymbol + ")";
							else
								newTransitionSymbol = outSymbol;
						}
						else if (inSymbol == "E" && outSymbol == "E")
						{
							newTransitionSymbol = "E";
						}
					}
				}

				m_transitions.UpdateTransitionSymbol(std::make_pair(inTransitions[i], directSymbol), outTransitions[o], newTransitionSymbol);
					
			} //----------------------------------------------------------------------------------------------------------------------------------
			else
			{
				//if there's no direct transition, we will create one 
				//check if there's a loop to self in the erase state
				if (m_transitions.ExistsTransitionBetweenStates(stateToErase, stateToErase))
				{
					//if there is, we will add the symbol of the transition to the direct transition
					std::string loopSymbol = m_transitions.GetTransitionSymbol(stateToErase, stateToErase);

					//let's check the symbols of the transitions
					if (loopSymbol != "E")
					{
						if (loopSymbol.size() > 1 && (loopSymbol[0] != '(' || loopSymbol[loopSymbol.size()-1] != ')') )
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*(" + outSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*" + outSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*(" + outSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*" + outSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol))
									newTransitionSymbol = "(" + inSymbol + ")(" + loopSymbol + ")*";
								else
									newTransitionSymbol = inSymbol + "(" + loopSymbol + ")*";
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + loopSymbol + ")*(" + outSymbol + ")";
								else
									newTransitionSymbol = "(" + loopSymbol + ")*" + outSymbol;
							}
							else if (inSymbol == "E" && outSymbol == "E")
								newTransitionSymbol = "(" + loopSymbol + ")*";
						}
						else if (loopSymbol.size() == 1)
						{
							if (inSymbol != "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*(" + outSymbol + ")";
								else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*" + outSymbol;
								else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*(" + outSymbol + ")";
								else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
									newTransitionSymbol = inSymbol + loopSymbol + "*" + outSymbol;
							}
							else if (inSymbol != "E" && outSymbol == "E")
							{
								if (NeedsParenthesis(inSymbol))
									newTransitionSymbol = "(" + inSymbol + ")" + loopSymbol + "*";
								else
									newTransitionSymbol = inSymbol + loopSymbol + "*";
							}
							else if (inSymbol == "E" && outSymbol != "E")
							{
								if (NeedsParenthesis(outSymbol))
									newTransitionSymbol = loopSymbol + "*(" + outSymbol + ")";
								else
									newTransitionSymbol = loopSymbol + "*" + outSymbol;
							}
						}
					}
					else //loopSymbol is "E" or null
					{
						if (inSymbol != "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + inSymbol + ")(" + outSymbol + ")";
							else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + inSymbol + ")" + outSymbol;
							else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
								newTransitionSymbol = inSymbol + "(" + outSymbol + ")";
							else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
								newTransitionSymbol = inSymbol + outSymbol;
						}
						else if (inSymbol != "E" && outSymbol == "E")
						{
							if (NeedsParenthesis(inSymbol))
								newTransitionSymbol = "(" + inSymbol + ")";
							else
								newTransitionSymbol = inSymbol;
						}
						else if (inSymbol == "E" && outSymbol != "E")
						{
							if (NeedsParenthesis(outSymbol))
								newTransitionSymbol = "(" + outSymbol + ")";
							else
								newTransitionSymbol = outSymbol;
						}
						else if (inSymbol == "E" && outSymbol == "E")
							newTransitionSymbol = "E";
					}
				}
				else
				{
					//if there's no loop to self, we will just add the symbols of the transitions
					//let's check the symbols of the transitions
					if (inSymbol != "E" && outSymbol != "E")
					{
						if (NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
							newTransitionSymbol = "(" + inSymbol + ")(" + outSymbol + ")";
						else if (NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
							newTransitionSymbol = "(" + inSymbol + ")" + outSymbol;
						else if (!NeedsParenthesis(inSymbol) && NeedsParenthesis(outSymbol))
							newTransitionSymbol = inSymbol + "(" + outSymbol + ")";
						else if (!NeedsParenthesis(inSymbol) && !NeedsParenthesis(outSymbol))
							newTransitionSymbol = inSymbol + outSymbol;
					}
					else if (inSymbol != "E" && outSymbol == "E")
					{
						if (NeedsParenthesis(inSymbol))
							newTransitionSymbol = "(" + inSymbol + ")";
						else
							newTransitionSymbol = inSymbol;
					}
					else if (inSymbol == "E" && outSymbol != "E")
					{
						if (NeedsParenthesis(outSymbol))
							newTransitionSymbol = "(" + outSymbol + ")";
						else
							newTransitionSymbol = outSymbol;
					}
					else if (inSymbol == "E" && outSymbol == "E")
						newTransitionSymbol = "E";
				}

				m_transitions.InsertTransition(inTransitions[i], newTransitionSymbol, { outTransitions[o] });
			}
		}
	}

	//erase the transitions through the state to be deleted
	for (int i = 0; i < inTransitions.size(); i++)
	{
		std::string inSymbol = m_transitions.GetTransitionSymbol(inTransitions[i], stateToErase);
		std::string backIn = m_transitions.GetTransitionSymbol(stateToErase, inTransitions[i]);

		m_transitions.DeleteTransition(std::make_pair(inTransitions[i], inSymbol), stateToErase);
		m_transitions.DeleteTransition(std::make_pair(stateToErase, backIn), inTransitions[i]);
	}

	for (int o = 0; o < outTransitions.size(); o++)
	{
		std::string outSymbol = m_transitions.GetTransitionSymbol(stateToErase, outTransitions[o]);
		std::string backOut = m_transitions.GetTransitionSymbol(outTransitions[o], stateToErase);

		m_transitions.DeleteTransition(std::make_pair(stateToErase, outSymbol), outTransitions[o]);
		m_transitions.DeleteTransition(std::make_pair(outTransitions[o], backOut), stateToErase);
	}

	std::string loopSymbol = m_transitions.GetTransitionSymbol(stateToErase, stateToErase);
	m_transitions.DeleteTransition(std::make_pair(stateToErase, loopSymbol), stateToErase);

	//erase the state to be deleted from m_states
	m_states.erase(std::remove(m_states.begin(), m_states.end(), stateToErase), m_states.end());

	//if there are no transitions left using "E" symbol, remove it from the alphabet
	if (!m_transitions.ExistsEpsilonTransiton())
	{
		m_alphabet.erase(std::remove(m_alphabet.begin(), m_alphabet.end(), 'E'), m_alphabet.end());
	}
}

Transitions NondeterministicFiniteAutomaton::GetTransitions()
{
	return m_transitions;
}

std::vector<std::string> NondeterministicFiniteAutomaton::GetStates()
{
	return m_states;
}

bool NondeterministicFiniteAutomaton::NeedsParenthesis(std::string transitionSymbol)
{
	if (transitionSymbol.find('+') != std::string::npos && transitionSymbol[0] != '(')
		return true;
	return false;
}
