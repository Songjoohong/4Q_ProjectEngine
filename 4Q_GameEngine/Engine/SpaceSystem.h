#pragma once
#include "ECS.h"
using namespace ECS;
class SpaceSystem : public EntitySystem,
	public EventSubscriber<Events::SpaceAssemble>,
	public EventSubscriber<Events::SpaceReturn>
{
	virtual void Configure(World* world) override;
	virtual void Deconfigure(World* world) override;

	virtual void Receive(World* world, const Events::SpaceAssemble& event) override;
	virtual void Receive(World* world, const Events::SpaceReturn& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;
};

