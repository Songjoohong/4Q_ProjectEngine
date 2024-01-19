#pragma once
#include "ECS.h"

namespace Component
{
	struct State
	{
		//ECS_DECLARE_TYPE

		virtual ~State() = default;

		virtual void Enter() {}
		virtual void Execute() {}
		virtual void Exit() {}
	};

	//ECS_DEFINE_TYPE(State)
}
