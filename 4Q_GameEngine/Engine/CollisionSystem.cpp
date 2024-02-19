#include "pch.h"
#include "CollisionSystem.h"
#include "Space.h"
#include "EntityIdentifier.h"
#include "PhysicsManager.h"

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
	world->each<Transform, BoxCollider>([&](Entity* ent, ComponentHandle<Transform> transform, ComponentHandle<BoxCollider> collider)
		{
			collider->m_IsRaycastHit = false;

			if(ent->has<EntityIdentifier>())
			{
				if (ent->get<EntityIdentifier>()->m_HasParent)
				{
					if (ent->m_parent->has<Transform>())
						collider->m_WorldPosition = (DirectX::SimpleMath::Matrix::CreateTranslation(collider->m_Center.ConvertToVector3()) * transform->m_RelativeMatrix.ConvertToMatrix() * ent->getParent()->get<Transform>()->m_WorldMatrix.ConvertToMatrix()).Translation();
				}
				else
				{
					collider->m_WorldPosition = (DirectX::SimpleMath::Matrix::CreateTranslation(collider->m_Center.ConvertToVector3()) * transform->m_RelativeMatrix.ConvertToMatrix()).Translation();
				}
			}
			

		});
}

