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
			if (staticMesh->m_FileName != "")
			{
				if(staticMesh->m_IsModelCreated)
					RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
				else
				{
					RenderManager::GetInstance()->CreateModel(staticMesh->m_FileName);
					staticMesh->m_IsModelCreated = true;
				}
			}
		});

	world->each<Transform, BoxCollider>([&](Entity* entity, ComponentHandle<Transform> transform,ComponentHandle<BoxCollider> boxCollider)->void
		{
			//RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
			RenderManager::GetInstance()->AddColliderBox(boxCollider->m_Center, boxCollider->m_Size);
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
