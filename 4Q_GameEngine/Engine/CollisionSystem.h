#pragma once
#include "BoxCollider.h"
#include "Script.h"
#include "ECS.h"
#include "Transform.h"

using namespace ECS;

class CollisionSystem : public EntitySystem,
	public EventSubscriber<Events::OnComponentAssigned<BoxCollider>>
{
public:
	CollisionSystem() = default;
	virtual ~CollisionSystem() override = default;

	virtual void Configure(World* world) override;
	virtual void Deconfigure(World* world) override;
	virtual void Receive(World* world, const Events::OnComponentAssigned<BoxCollider>& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;

};

