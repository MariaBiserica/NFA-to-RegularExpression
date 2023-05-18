#include "NondeterministicFiniteAutomaton.h"
#include <fstream>

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton()
{
}

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton(std::vector<std::string> states, std::string alphabet, Transitions transitions, std::string initialState, std::vector<std::string> finalStates)
    :m_states(states),
    m_alphabet(alphabet),
    m_transitions(transitions),
    m_initialState(initialState),
    m_finalStates(finalStates)
{
}

void NondeterministicFiniteAutomaton::ReadNFA()
{
	std::ifstream fin("inputNFA.txt");
	if (!fin.is_open())
	{
		std::cout << "Error opening file!\n";
		return;
	}
    else
    {
        std::string line;
        std::getline(fin, line);
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        while ((pos = line.find(delimiter)) != std::string::npos)
        {
            token = line.substr(0, pos);
            m_states.push_back(token);
            line.erase(0, pos + delimiter.length());
        }
        m_states.push_back(line);
        

        std::getline(fin, line);
        m_alphabet = line;

        std::getline(fin, line);
        m_initialState = line;

        std::getline(fin, line);
        pos = 0;
        while ((pos = line.find(delimiter)) != std::string::npos)
        {
            token = line.substr(0, pos);
            m_finalStates.push_back(token);
            line.erase(0, pos + delimiter.length());
        }
		m_finalStates.push_back(line);

        while (std::getline(fin, line))
        {
            //mai intai luam tokenii de pe linie
            std::vector<std::string> tokens;
			pos = 0;
            while ((pos = line.find("-")) != std::string::npos)
            {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);

            std::string transitionState = tokens[0];
            std::string transitionSymbol = tokens[1];
            std::vector<std::string> transitionResultStates;
			transitionResultStates.push_back(tokens[2]);

            m_transitions.InsertTransition(transitionState, transitionSymbol, transitionResultStates);
        }
    }

	fin.close();
}

void NondeterministicFiniteAutomaton::PrintAutomaton()
{
    std::cout << "Elementele automatului sunt: " << std::endl;
    std::cout << "Stari - Q:{";
    for (int i = 0; i < m_states.size(); i++)
    {
        if (i != (m_states.size() - 1))
            std::cout << m_states[i] << ", ";
        else
            std::cout << m_states[i] << "}" << std::endl;
    }

    std::cout << "Alfabet de intrare - Sigma:{";
    for (int i = 0; i < m_alphabet.size(); i++)
    {
        if (i != (m_alphabet.size() - 1))
            std::cout << m_alphabet[i] << ", ";
        else
            std::cout << m_alphabet[i] << "}" << std::endl;
    }

    std::cout << "Stare initiala: " << m_initialState << std::endl;

    std::cout << "Multimea starilor finale - F:{";
    for (int i = 0; i < m_finalStates.size(); i++)
    {
        if (i != (m_finalStates.size() - 1))
            std::cout << m_finalStates[i] << ", ";
        else
            std::cout << m_finalStates[i] << "}" << std::endl;
    }

    std::cout << "Functia de tranzitie:" << std::endl;
    m_transitions.PrintTransitions();
}

Transitions NondeterministicFiniteAutomaton::GetTransitions()
{
    return m_transitions;
}

std::vector<std::string> NondeterministicFiniteAutomaton::GetStates()
{
    return m_states;
}

std::string NondeterministicFiniteAutomaton::GetInitialState()
{
    return m_initialState;
}

std::vector<std::string> NondeterministicFiniteAutomaton::GetFinalStates()
{
    return m_finalStates;
}

std::string NondeterministicFiniteAutomaton::GetAlphabet()
{
    return m_alphabet;
}
