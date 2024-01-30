#include "pch.h"
#include "ECS.h"
#include "RenderSystem.h"
#include "StaticMesh.h"
#include "Transform.h"

#include "RenderManager.h"

using namespace ECS;

void RenderSystem::Configure(ECS::World* world)
{
	world->Subscribe<ECS::Events::OnComponentAssigned< StaticMesh>>(this);
	world->Subscribe<ECS::Events::OnComponentAssigned< SkinnedMesh>>(this);
}

void RenderSystem::Deconfigure(ECS::World* world)
{
	world->UnsubscribeAll(this);
}

void RenderSystem::Tick(ECS::World* world, ECS::DefaultTickData data)
{
	Vector3D p = Vector3D(0.f, 0.f, 100.f);
	Vector3D r = Vector3D(0.f, 0.f, 0.f);
	Vector3D s = Vector3D(1.f, 1.f, 1.f);
	world->each<StaticMesh, Transform>([&](Entity* entity, const ComponentHandle< StaticMesh> mesh, ComponentHandle< Transform> transform)->void{
			RenderManager::GetInstance()->AddStaticMesh("FBXLoad_Test/fbx/zeldaPosed001.fbx", p, r, s);
		});
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned< SkinnedMesh>& event)
{

}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned< StaticMesh>& event)
{
	
	RenderManager::GetInstance()->CreateModel("FBXLoad_Test/fbx/zeldaPosed001.fbx");
}
