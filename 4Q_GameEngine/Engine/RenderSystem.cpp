#include "pch.h"
#include "ECS.h"
#include "RenderSystem.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "EntityIdentifier.h"
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
				if (entity->get<EntityIdentifier>()->m_HasParent)
				{
					auto matrix = transform->m_RelativeMatrix.ConvertToMatrix()* entity->getParent()->get<Transform>()->m_WorldMatrix.ConvertToMatrix();
					SimpleMath::Vector3 scale, pos;
					SimpleMath::Quaternion rot;
					matrix.Decompose(scale, rot, pos);
					staticMesh->m_BoundingBox.Center = { pos.x,pos.y + staticMesh->m_BoundingBox.Extents.y/2,pos.z };
				}
				else
				{
					staticMesh->m_BoundingBox.Center = transform->m_Position.ConvertToVector3();
				}
				if (staticMesh->m_IsModelCreated)
				{
					
					if(RenderManager::GetInstance()->Culling(staticMesh->m_BoundingBox))
						RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
						RenderManager::GetInstance()->AddBoundingBox(staticMesh->m_BoundingBox);
				}
				else
				{
					RenderManager::GetInstance()->CreateModel(staticMesh->m_FileName, staticMesh->m_BoundingBox);
					staticMesh->m_IsModelCreated = true;
				}
			}
		});

	world->each<Transform, BoxCollider>([&](Entity* entity, ComponentHandle<Transform> transform, ComponentHandle<BoxCollider> boxCollider)->void
		{
			//RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
			RenderManager::GetInstance()->AddColliderBox(boxCollider->m_WorldPosition, boxCollider->m_Size, transform->m_Rotation);
		});
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<StaticMesh>& event)
{
	// minjeong : fbx load test
	if (event.component->m_FileName != "")
	{
		RenderManager::GetInstance()->CreateModel(event.component->m_FileName,event.component->m_BoundingBox);
		event.component->m_IsModelCreated = true;
	}
}
