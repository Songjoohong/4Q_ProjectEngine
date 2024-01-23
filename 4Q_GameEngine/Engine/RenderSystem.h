#pragma once
#include "ECS.h"
#include "SkinnedMesh.h"
#include "StaticMesh.h"

class RenderSystem : public ECS::EntitySystem,
	public ECS::EventSubscriber<ECS::Events::OnComponentAssigned<Component::StaticMesh>>,
	public ECS::EventSubscriber<ECS::Events::OnComponentAssigned<Component::SkinnedMesh>>
{
public:
	virtual ~RenderSystem() override = default;

	virtual void Configure(ECS::World* world) override;
	virtual void Deconfigure(ECS::World* world) override;

	virtual void Tick(ECS::World* world, ECS::DefaultTickData data) override;

	virtual void Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Component::SkinnedMesh>& event) override;
	virtual void Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Component::StaticMesh>& event) override;
};

