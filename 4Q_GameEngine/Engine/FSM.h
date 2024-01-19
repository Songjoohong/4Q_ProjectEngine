#pragma once
#include <typeindex>
#include <unordered_map>

class State;

namespace Component
{
	struct FSM
	{
		State* m_CurrentState;

		void AddState(ECS::ComponentHandle<State> state)
		{
			//m_States[typeid(State)] = ECS::ComponentHandle<State>;
		}

	};
}
