#pragma once
#include "ECS.h"
#include "Script.h"

using namespace ECS;

class ScriptSystem : public EntitySystem,
	public EventSubscriber<Events::OnEntityDestroyed>,
	public EventSubscriber<Events::OnComponentAssigned<Script>>
{
	virtual ~ScriptSystem() override = default;

	virtual void Configure(World* world) override
	{
		world->Subscribe<Events::OnComponentAssigned<Script>>(this);
		world->Subscribe<Events::OnEntityDestroyed>(this);

	}

	virtual void Deconfigure(World* world) override
	{
		world->UnsubscribeAll(this);
	}

	virtual void Receive(class World* world, const Events::OnComponentAssigned<Script>& event) override
	{
		std::cout << "An entity was created!" << std::endl;
		if(event.entity->has<Script>())
			event.entity->get<Script>()->Awake();
	}

	virtual void Receive(class World* world, const Events::OnEntityDestroyed& event) override
	{
		std::cout << "An entity was destroyed!" << std::endl;
		if(event.entity->has<Script>() == true)
		{
			event.entity->get<Script>()->OnDestroyed();
		}

	}

	virtual void Tick(World* world, float deltaTime) override
	{
		world->each<Script>([&](Entity* entity, ComponentHandle<Script> script)->void
			{
				script->Update(deltaTime);
			});
	}
};

