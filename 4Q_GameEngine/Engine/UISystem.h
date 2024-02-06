#pragma once
#include "ECS.h"

using namespace ECS;

class UISystem : public EntitySystem
{
public:
	UISystem() = default;
	virtual ~UISystem() = default;

	virtual void Configure(World* world) override
	{
		
	}

	virtual void Deconfigure(World* world) override
	{
		
	}

	virtual void Tick(World* world, ECS::DefaultTickData data) override;
};

