#include "pch.h"
#include "RenderSystem.h"

#include "RenderManager.h"

void RenderSystem::Configure(ECS::World* world)
{
	world->Subscribe<ECS::Events::OnComponentAssigned<Component::StaticMesh>>(this);
	world->Subscribe<ECS::Events::OnComponentAssigned<Component::SkinnedMesh>>(this);
}

void RenderSystem::Deconfigure(ECS::World* world)
{
	world->UnsubscribeAll(this);
}

void RenderSystem::Tick(ECS::World* world, ECS::DefaultTickData data)
{
	
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Component::SkinnedMesh>& event)
{

}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Component::StaticMesh>& event)
{
	RenderManager::GetInstance()->AddStaticMesh("FBXLoad_Test/fbx/box.fbx", Vector3D(0.f, 0.f, 100.f), Vector3D(0.f, 0.f, 0.f));
}
