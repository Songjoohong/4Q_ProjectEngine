#pragma once
#include "ECS.h"

#include "Sound.h"
using namespace ECS;

class SoundSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<Sound>>,
	public EventSubscriber<Events::OnEntityDestroyed>
{
	virtual void Configure(World* world) override;
	

	virtual void Deconfigure(World* world) override;
	
	virtual void Receive(World* world, const Events::OnComponentAssigned<Sound>& event)		override;

	virtual void Receive(World* world, const Events::OnEntityDestroyed& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;
};

