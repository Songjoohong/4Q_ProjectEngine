#pragma once
#include <typeindex>
#include <unordered_map>

class State;

namespace Component
{
	struct FSM
	{
		State* m_CurrentState;
		std::unordered_map<const char*, State*> m_States;
		void AddState(ECS::ComponentHandle<State> state)
		{
			m_States[state->m_Name] = &state.get();
		}

		void ChangeState(const char* stateName)
		{
			m_CurrentState = m_States[stateName];
		}
	};
}
