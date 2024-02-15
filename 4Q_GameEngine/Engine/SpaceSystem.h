#pragma once
#include "ECS.h"
using namespace ECS;
class SpaceSystem : public EntitySystem,
	public EventSubscriber<Events::SpaceAssemble>
{
	virtual void Configure(World* world) override;
	virtual void Deconfigure(World* world) override;

	virtual void Receive(World* world, const Events::SpaceAssemble& event) override;
};
