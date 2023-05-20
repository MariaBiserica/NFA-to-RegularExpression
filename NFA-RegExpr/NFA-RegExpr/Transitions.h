#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <functional>
#include <queue>
#include <unordered_set>

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ h2;
	}
};

using TransitionFunctionInputs = std::pair<std::string, std::string>;
using Unordered_map = std::unordered_map<TransitionFunctionInputs, std::vector<std::string>, pair_hash>;

class Transitions
{
public:
	void PrintTransitions();
	
	void InsertTransition(std::string transitionState, std::string transitionSymbol, std::vector<std::string> transitionResultStates);
	void UpdateTransitionSymbol(TransitionFunctionInputs transitionOldInputs, std::string transitionFinalState, std::string newTransitionSymbol);
	void DeleteTransition(TransitionFunctionInputs transitionInputs, std::string transitionFinalState);

	bool ExistsEpsilonTransiton();
	bool ExistsTransition(std::string transitionState, std::string transitionSymbol);
	bool ExistsTransitionBetweenStates(std::string inState, std::string outState);

	int GetInNumberOfTransitions(std::string state);
	int GetOutNumberOfTransitions(std::string state);
	std::vector<std::string> GetInTransitions(std::string state);
	std::vector<std::string> GetOutTransitions(std::string state);

	std::string GetTransitionSymbol(std::string state1, std::string state2);
	
	Unordered_map GetDeltaFunction();

private:
	bool ExistsState(std::string state);
	bool ExistsSymbol(std::string symbol);

private:
	Unordered_map m_delta;
	std::vector<std::string> m_states;
	std::string m_symbols;
};