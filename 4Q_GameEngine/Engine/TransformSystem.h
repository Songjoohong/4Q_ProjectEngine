#pragma once
#include "ECS.h"
#include "Transform.h"
using namespace ECS;
class TransformSystem : public EntitySystem, public ECS::EventSubscriber<ECS::Events::OnComponentAssigned<Transform>>
{
public:
	TransformSystem() = default;
	virtual ~TransformSystem() = default;

	virtual void Configure(ECS::World* world) override;
	virtual void Deconfigure(ECS::World* world) override;
	virtual void Tick(World* world, ECS::DefaultTickData data) override;
	virtual void Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Transform>& event) override;
};

