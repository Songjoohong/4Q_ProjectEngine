#pragma once
#include "ECS.h"
#include "Sprite2D.h"

using namespace ECS;

class SpriteSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<Sprite2D>>,
	public EventSubscriber<Events::OnEntityDestroyed>
{
	virtual void Configure(World* world) override;
	virtual void Deconfigure(World* world) override;

	virtual void Receive(World* world, const Events::OnComponentAssigned<Sprite2D>& event) override;
	virtual void Receive(World* world, const Events::OnEntityDestroyed& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;
};
