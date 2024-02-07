#include "pch.h"
#include "CollisionSystem.h"

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
	event.component->m_Center = event.entity->get<Transform>()->m_Position;
	PhysicsManager::GetInstance()->CreateCollider(event.component.component, event.entity->getEntityId());
}

void CollisionSystem::Tick(World* world, ECS::DefaultTickData data)
{
	world->each<Transform, BoxCollider>([&](Entity* ent, ComponentHandle<Transform> transform, ComponentHandle<BoxCollider> collider)
		{
			transform->m_Position = collider->m_Center;
			//transform->m_Rotation = collider->m_Rotation;
		});
}
