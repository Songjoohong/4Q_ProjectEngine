#include "pch.h"
#include "CollisionSystem.h"
#include "Space.h"
#include "EntityIdentifier.h"
#include "PhysicsManager.h"
#include "PlayerInformation.h"

void CollisionSystem::Configure(World* world)
{
	world->Subscribe<Events::OnComponentAssigned<BoxCollider>>(this);
}

void CollisionSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void CollisionSystem::Receive(World* world, const Events::OnComponentAssigned<BoxCollider>& event)
{ 
	event.component->m_WorldPosition = event.entity->get<Transform>()->m_Position + event.component->m_Center;
	event.component->m_Rotation = event.entity->get<Transform>()->m_Rotation; // rotation test
	event.component->m_Size = event.entity->get<Transform>()->m_Scale;
	PhysicsManager::GetInstance()->CreateCollider(event.component.component, event.entity->getEntityId());
}

void AddParentPositionToChildren(Entity* entity, const Vector3D& parentPosition)
{
	auto transform = entity->get<Transform>();
	auto collider = entity->get<BoxCollider>();

	if (transform && collider) {
		collider->m_Center = parentPosition + transform->m_Position;

	}

	for (auto& child : entity->m_children)
	{
		// 자식의 자식에게도 더해주기 위해 재귀 호출
		AddParentPositionToChildren(child, collider->m_Center);
	}
}

void CollisionSystem::Tick(World* world, ECS::DefaultTickData data)
{
	Entity* player = nullptr;
	ComponentHandle<PlayerInformation> info;

	world->each<PlayerInformation>([&](Entity* ent, ComponentHandle<PlayerInformation> playerInfo)->void
		{
			player = ent;
			info = playerInfo;
		});

	world->each<Transform, BoxCollider>([&](Entity* ent, ComponentHandle<Transform> transform, ComponentHandle<BoxCollider> collider)
		{
			collider->m_Rotation = transform->m_Rotation;
			//collider->m_Size = transform->m_Scale;

			if(collider->m_IsRaycastHit)
			{
				if(player)
				{
					if(player->has<PlayerInformation>())
					{
						info->m_LookingEntity = ent->get<EntityIdentifier>()->m_EntityName;
						std::cout << info->m_LookingEntity << std::endl;
					}
				}
			}
			collider->m_WasRaycastHit = collider->m_IsRaycastHit;
			collider->m_IsRaycastHit = false;

			if(info)
			{
				if (collider->m_State == CollisionState::ENTER)
				{
					info->m_CollidingEntities.push_back(ent->get<EntityIdentifier>()->m_EntityName);
				}
				if (collider->m_State == CollisionState::EXIT)
				{
					info->m_CollidingEntities.erase(std::remove_if(info->m_CollidingEntities.begin(), info->m_CollidingEntities.end(), [&](const string& str) { return str == ent->get<EntityIdentifier>()->m_EntityName; }), info->m_CollidingEntities.end());
				}
			}
			

			if (ent->get<EntityIdentifier>()->m_HasParent)
			{
				if (ent->m_parent->has<Transform>())
					collider->m_Rotation += ent->getParent()->get<Transform>()->m_Rotation;
			}
			
				if (collider->m_CollisionType == CollisionType::PLAYER)
				{
					XMVECTOR determinant;
					transform->m_Position = (DirectX::SimpleMath::Matrix::CreateTranslation(collider->m_WorldPosition.ConvertToVector3()) * XMMatrixInverse(&determinant, DirectX::SimpleMath::Matrix::CreateTranslation(collider->m_Center.ConvertToVector3()))).Translation();
				}
				else
					collider->m_WorldPosition = (DirectX::SimpleMath::Matrix::CreateTranslation(collider->m_Center.ConvertToVector3()) * transform->m_WorldMatrix.ConvertToMatrix()).Translation();

		});
}

