#pragma once
#include "ECS.h"
#include "FSM.h"

#include "State.h"

using namespace ECS;

class FSMSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<Component::State>>
{
	virtual ~FSMSystem() override = default;

	virtual void Configure(World* world) override
	{
		world->subscribe<Events::OnComponentAssigned<Component::State>>(this);
	}

	virtual void Deconfigure(World* world) override
	{
		world->unsubscribeAll(this);
	}

	virtual void Receive(World* world, const Events::OnComponentAssigned<Component::State>& event) override
	{
		const auto fsm = event.entity->get<Component::FSM>();
		const auto state = event.component;
		fsm->AddState(state);
	}

	virtual void Tick(World* world, DefaultTickData data) override
	{
		
	}


};

