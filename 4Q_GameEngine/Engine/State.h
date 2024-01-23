#pragma once
#include "ECS.h"


struct State
{
	
	virtual ~State() = default;

	virtual void Enter() {}
	virtual void Execute() {}
	virtual void Exit() {}

	const char* m_Name = "";
};

