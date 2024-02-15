#include "pch.h"
#include "SpaceSystem.h"

void SpaceSystem::Configure(World* world)
{
	world->Subscribe<Events::SpaceAssemble>(this);
}

void SpaceSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void SpaceSystem::Receive(World* world, const Events::SpaceAssemble& event)
{
	
}

