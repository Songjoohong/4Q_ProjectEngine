#include "pch.h"
#include "ECS.h"
#include "RenderSystem.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "BoxCollider.h"

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
	world->each<StaticMesh, Transform>([&](Entity* entity, const ComponentHandle<StaticMesh> staticMesh, ComponentHandle<Transform> transform)->void
		{
			RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
		});
	world->each<StaticMesh, Transform,BoxCollider>([&](Entity* entity, const ComponentHandle<StaticMesh> staticMesh, ComponentHandle<Transform> transform,ComponentHandle<BoxCollider> boxCollider)->void
		{
			RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
			RenderManager::GetInstance()->AddColliderBox(boxCollider->m_Center, boxCollider->m_Size, boxCollider->m_IsTrigger);
		});
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<StaticMesh>& event)
{
	// minjeong : fbx load test
	RenderManager::GetInstance()->CreateModel(event.component->m_FileName);
}
