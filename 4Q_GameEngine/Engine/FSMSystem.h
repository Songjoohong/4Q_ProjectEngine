#pragma once
#include "ECS.h"
#include "FSM.h"

#include "State.h"

using namespace ECS;

class FSMSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<State>>
{
	virtual ~FSMSystem() override = default;

	virtual void Configure(World* world) override
	{
		world->subscribe<Events::OnComponentAssigned<State>>(this);
	}

	virtual void Deconfigure(World* world) override
	{
		world->unsubscribeAll(this);
	}

	virtual void Receive(World* world, const Events::OnComponentAssigned<State>& event) override
	{
		assert(event.entity->has<Component::FSM>());

		const auto fsm = event.entity->get<Component::FSM>();
		const auto state = event.component;
		fsm->AddState(state);
	}

	virtual void Tick(World* world, DefaultTickData data) override
	{
		
	}


};

