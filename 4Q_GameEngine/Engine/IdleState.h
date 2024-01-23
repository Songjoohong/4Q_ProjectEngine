#pragma once
#include "FSM.h"
#include "State.h"

struct IdleState : public State
{
	ECS_DECLARE_TYPE

	explicit IdleState(Entity* ent)
		: State(ent)
	{}
	virtual ~IdleState() override = default;

	virtual void Enter() override
	{

	}

	virtual void Execute() override
	{
		
	}

	virtual void Exit() override
	{
		
	}


};
ECS_DEFINE_TYPE(IdleState)
