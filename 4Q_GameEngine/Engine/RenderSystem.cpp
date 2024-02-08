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
			if (collider->m_FileName != "")
			{
				if(collider->m_IsModelCreated)
					RenderManager::GetInstance()->AddStaticMesh(collider->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
				else
				{
					RenderManager::GetInstance()->CreateModel(collider->m_FileName);
					collider->m_IsModelCreated = true;
				}
			}
		});
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<StaticMesh>& event)
{
	// minjeong : fbx load test
	if (event.component->m_FileName != "")
	{
		RenderManager::GetInstance()->CreateModel(event.component->m_FileName);
		event.component->m_IsModelCreated = true;
	}
}
