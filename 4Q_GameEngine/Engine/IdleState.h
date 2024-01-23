#pragma once
#include "FSM.h"
#include "State.h"

struct IdleState : public State
{
	ECS_DECLARE_TYPE
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
