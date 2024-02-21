#include "pch.h"
#include "ECS.h"
#include "RenderSystem.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Light.h"
#include "RenderManager.h"

using namespace ECS;

void RenderSystem::Configure(ECS::World* world)
{
	world->Subscribe<ECS::Events::OnComponentAssigned<StaticMesh>>(this);
	world->Subscribe<ECS::Events::OnComponentAssigned<Light>>(this);
	world->Subscribe<ECS::Events::OnEntityDestroyed>(this);
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
				if (staticMesh->m_IsModelCreated)
					RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
				else
				{
					RenderManager::GetInstance()->CreateModel(staticMesh->m_FileName);
					staticMesh->m_IsModelCreated = true;
				}
			}
		});

	world->each<Transform, BoxCollider>([&](Entity* entity, ComponentHandle<Transform> transform, ComponentHandle<BoxCollider> boxCollider)->void
		{
			//RenderManager::GetInstance()->AddStaticMesh(staticMesh->m_FileName, transform->m_WorldMatrix.ConvertToMatrix());
			RenderManager::GetInstance()->AddColliderBox(boxCollider->m_Center, boxCollider->m_Size, transform->m_WorldMatrix.ConvertToMatrix());
		});

	world->each<Light, Transform>([&](Entity* ent, ComponentHandle<Light> light, ComponentHandle<Transform> transform)->void
		{
			if (light->m_Type == LightType::Point)
			{
				RenderManager::GetInstance()->EditPointLight(ent->getEntityId(), transform->m_WorldMatrix.ConvertToMatrix().Translation(), light->m_Color, light->m_Intensity, light->m_Radius);
			}
			else
			{
				RenderManager::GetInstance()->EditDirectionalLight(light->m_Direction.ConvertToVector3(), light->m_Color.ConvertToVector3());
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

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnComponentAssigned<Light>& event)
{
	auto pos = event.entity->get<Transform>()->m_WorldMatrix.ConvertToMatrix().Translation();
	if (event.component->m_Type == LightType::Point)
	{
		RenderManager::GetInstance()->CreateLight(event.entity->getEntityId(), pos, event.component->m_Color, event.component->m_Intensity, event.component->m_Radius);
	}
}

void RenderSystem::Receive(ECS::World* world, const ECS::Events::OnEntityDestroyed& event)
{
	if (event.entity->has<Light>())
	{
		RenderManager::GetInstance()->DeletePointLight(event.entity->getEntityId());
	}
}
