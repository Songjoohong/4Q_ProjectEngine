#pragma once
#include "ECS.h"
#include "FSM.h"
#include "Movement.h"

using namespace ECS;

class FSMSystem : public EntitySystem
{
	virtual ~FSMSystem() override = default;

	virtual void Tick(World* world, DefaultTickData data) override
	{
		world->each<FSM, Movement>([&](Entity* entity, ComponentHandle<FSM> fsm, ComponentHandle<Movement> movement)->void
			{

			});
	}


};

