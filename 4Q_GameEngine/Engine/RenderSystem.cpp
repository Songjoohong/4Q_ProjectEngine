#include "pch.h"
#include "ECS.h"
#include "RenderSystem.h"
#include "StaticMesh.h"
#include "Transform.h"

#include "RenderManager.h"

using namespace ECS;

void RenderSystem::Configure(ECS::World* world)
{
	world->Subscribe<ECS::Events::OnComponentAssigned<StaticMesh>>(this);
	world->Subscribe<ECS::Events::OnComponentAssigned<SkinnedMesh>>(this);
}

void RenderSystem::Deconfigure(ECS::World* world)
{
	world->UnsubscribeAll(this);
}

void RenderSystem::Tick(ECS::World* world, ECS::DefaultTickData data)
{
	
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<::SkinnedMesh>& event)
{

}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<StaticMesh>& event)

{
	
}
