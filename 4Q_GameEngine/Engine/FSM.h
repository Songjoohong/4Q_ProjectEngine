#pragma once
#include <typeindex>
#include <unordered_map>

class State;

struct FSM
{
	State* m_CurrentState;
	std::unordered_map<std::string , State*> m_States;
	void AddState(ECS::ComponentHandle<State> state)
	{
		m_States[state->GetName()] = &state.get();
	}

	void ChangeState(std::string stateName)
	{
		m_CurrentState = m_States[stateName];
	}
};

