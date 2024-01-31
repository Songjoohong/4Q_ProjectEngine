#pragma once
#include "ECS.h"
#include "Debug.h"


using namespace ECS;

class DebugSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<Debug>>,
	public EventSubscriber<Events::OnEntityDestroyed>
{
	virtual void Configure(World* world) override;

	virtual void Deconfigure(World* world) override;

	virtual void Receive(World* world, const Events::OnComponentAssigned<Debug>& event) override;
	virtual void Receive(World* world, const Events::OnEntityDestroyed& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;

};

