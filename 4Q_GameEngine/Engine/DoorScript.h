#pragma once
#include "ECS.h"
#include "Script.h"

struct DoorScript : public Script
{
	DoorScript(Entity* ent)
		:Script(ent)
	{}
	virtual ~DoorScript() = default;

	virtual void Update(float deltaTime) override
	{
		
	}
};