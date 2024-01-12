#pragma once
#include "ECS.h"
#include "Script.h"

using namespace ECS;

class ScriptSystem : public EntitySystem,
	public EventSubscriber<Events::OnEntityCreated>,
	public EventSubscriber<Events::OnEntityDestroyed>
{
	virtual ~ScriptSystem() override = default;

	virtual void Configure(World* world) override
	{
		world->subscribe<Events::OnEntityCreated>(this);
		world->subscribe<Events::OnEntityDestroyed>(this);

	}

	virtual void Deconfigure(World* world) override
	{
		world->unsubscribeAll(this);
	}

	virtual void receive(class World* world, const Events::OnEntityCreated& event) override
	{
		std::cout << "An entity was created!" << std::endl;
		world->each<Script>([&](Entity* entity, ComponentHandle<Script> script)->void
			{
				script->Awake();
			});
	}

	virtual void receive(class World* world, const Events::OnEntityDestroyed& event) override
	{
		std::cout << "An entity was destroyed!" << std::endl;
		world->each<Script>([&](Entity* entity, ComponentHandle<Script> script)->void
			{
				script->OnDestroyed();
			});

	}

	virtual void Tick(World* world, ECS::DefaultTickData data) override
	{
		world->each<Script>([&](Entity* entity, ComponentHandle<Script> script)->void
			{
				script->Update();
			});
	}
};

