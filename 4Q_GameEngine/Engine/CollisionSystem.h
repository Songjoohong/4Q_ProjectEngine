#pragma once
#include "BoxCollider.h"
#include "Script.h"
#include "ECS.h"
#include "Transform.h"

using namespace ECS;

class CollisionSystem : public EntitySystem,
	public EventSubscriber<Events::OnEntityCreated>,
	public EventSubscriber<Events::OnEntityDestroyed>
{
public:
	virtual ~CollisionSystem() override = default;

	virtual void Configure(World* world) override
	{
		world->subscribe<Events::OnEntityCreated>(this);
		world->subscribe<Events::OnEntityDestroyed>(this);
	}

	virtual void Deconfigure(World* world) override
	{
		world->unsubscribeAll(this);
	}

	

	bool CheckCollision(ComponentHandle<Component::BoxCollider> lhs, ComponentHandle<Component::BoxCollider> rhs)
	{
		return true; // 게임에 따라 AABB만 사용할지, SAT 사용할지 정해질 듯
	}

	virtual void Tick(World* world, ECS::DefaultTickData data) override
	{
		world->each<Component::BoxCollider, Component::Transform>([&](Entity* entity,const ComponentHandle<Component::BoxCollider> collider, ComponentHandle<Component::Transform> transform)->void
			{
				collider->m_Center = transform->m_Position;
				for (const auto& rhs : m_Entities)
				{
					if (entity == rhs)
						continue;

					if (CheckCollision(collider, rhs->get<Component::BoxCollider>()))
					{
						if (collider->m_CurrentState == Component::CollisionState::NONE || collider->m_CurrentState == Component::CollisionState::EXIT)
						{
							entity->get<Component::Script>()->OnCollisionEnter();
							collider->m_CurrentState = Component::CollisionState::ENTER;
						}
						else
						{
							entity->get<Component::Script>()->OnCollisionStay();
							collider->m_CurrentState = Component::CollisionState::STAY;
						}
					}
					else
					{
						if(collider->m_CurrentState == Component::CollisionState::ENTER || collider->m_CurrentState == Component::CollisionState::STAY)
						{
							entity->get<Component::Script>()->OnCollisionExit();
							collider->m_CurrentState = Component::CollisionState::EXIT;
						}
						else
						{
							collider->m_CurrentState = Component::CollisionState::NONE;
						}

					}

				}
			});
	}

	virtual void Receive(World* world, const Events::OnEntityCreated& event) override
	{
		if(event.entity->has<Component::BoxCollider>())
		{
			m_Entities.push_back(event.entity);
		}
	}

	virtual void Receive(World* world, const Events::OnEntityDestroyed& event) override
	{
		if(event.entity->has<Component::BoxCollider>())
		{
			m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(), [&event](const Entity* entity)
				{
					return entity == event.entity;
				}), m_Entities.end());
		}
	}
private:
	std::vector<Entity*> m_Entities;
};

