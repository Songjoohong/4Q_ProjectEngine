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
		world->Subscribe<Events::OnEntityCreated>(this);
		world->Subscribe<Events::OnEntityDestroyed>(this);
	}

	virtual void Deconfigure(World* world) override
	{
		world->UnsubscribeAll(this);
	}

	


	bool CheckCollision(ComponentHandle<BoxCollider> lhs, ComponentHandle<BoxCollider> rhs)
	{
		return true; // 게임에 따라 AABB만 사용할지, SAT 사용할지 정해질 듯
	}

	virtual void Tick(World* world, ECS::DefaultTickData data) override
	{
		world->each<BoxCollider, Transform>([&](Entity* entity,const ComponentHandle<BoxCollider> collider, ComponentHandle<Transform> transform)->void

			{
				collider->m_Center = transform->m_Position;
				for (const auto& rhs : m_Entities)
				{
					if (entity == rhs)
						continue;

					if (CheckCollision(collider, rhs->get<BoxCollider>()))
					{
						if (collider->m_CurrentState == CollisionState::NONE || collider->m_CurrentState == CollisionState::EXIT)
						{
							entity->get<Script>()->OnCollisionEnter();
							collider->m_CurrentState = CollisionState::ENTER;
						}
						else
						{
							entity->get<Script>()->OnCollisionStay();

							collider->m_CurrentState = CollisionState::STAY;
						}
					}
					else
					{
						if(collider->m_CurrentState == CollisionState::ENTER || collider->m_CurrentState == CollisionState::STAY)
						{
							entity->get<Script>()->OnCollisionExit();
							collider->m_CurrentState = CollisionState::EXIT;
						}
						else
						{
							collider->m_CurrentState = CollisionState::NONE;

						}

					}

				}
			});
	}

	virtual void Receive(World* world, const Events::OnEntityCreated& event) override
	{
		if(event.entity->has<BoxCollider>())
		{
			m_Entities.push_back(event.entity);
		}
	}

	virtual void Receive(World* world, const Events::OnEntityDestroyed& event) override
	{
		if(event.entity->has<BoxCollider>())
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

