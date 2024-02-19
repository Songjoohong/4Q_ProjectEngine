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
	event.component->m_WorldPosition = event.entity->get<Transform>()->m_Position;
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
			transform->m_Position = collider->m_WorldPosition - collider->m_Center;
			collider->m_IsRaycastHit = false;

			//if (ent->get<EntityIdentifier>()->m_HasParent)
			//{
			//	collider->m_Center = ent->m_parent->get<Transform>()->m_Position + transform->m_Position;
			//}
			//else
			//{
			//	collider->m_Center = transform->m_Position;	// TODO: 24.02.16 임시 수정
			//}

			//for (auto& child : ent->m_children)
			//{
			//	AddParentPositionToChildren(child, collider->m_Center);
			//}
			//collider->m_Center = transform->m_Position;

			//if (ent->get<EntityIdentifier>()->m_HasParent)
			//{
			//	if(ent->m_parent->has<Transform>())
			//		collider->m_Center = ent->getParent()->get<Transform>()->m_Position + transform->m_Position;
			//}
			//else
			//{
			//	//collider->m_Center = transform->m_Position;
			//	collider->m_Center = transform->m_WorldMatrix.ConvertToMatrix().Translation();
			//}

			/*for (auto& child : ent->m_children)
			{
				AddParentPositionToChildren(child, collider->m_Center);
			}*/

			//transform->m_Rotation = collider->m_Rotation;
		});
}

