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
}

void RenderSystem::Deconfigure(ECS::World* world)
{
	world->UnsubscribeAll(this);
}

void RenderSystem::Tick(ECS::World* world, ECS::DefaultTickData data)
{
	world->each<StaticMesh, Transform>([&](Entity* entity, const ComponentHandle<StaticMesh> collider, ComponentHandle<Transform> transform)->void
		{
			RenderManager::GetInstance()->AddStaticMesh(collider->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
		});
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<StaticMesh>& event)
{

	RenderManager::GetInstance()->CreateModel("FBXLoad_Test/fbx/plane.fbx");
	RenderManager::GetInstance()->CreateModel("FBXLoad_Test/fbx/Character.fbx");
	RenderManager::GetInstance()->CreateModel("FBXLoad_Test/fbx/zeldaPosed001.fbx");

}
