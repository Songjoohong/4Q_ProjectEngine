#pragma once
#include "ECS.h"

//ECS_TYPE_IMPLEMENTATION

   using namespace ECS;

class MovementSystem : public ECS::EntitySystem
{
public:
	virtual ~MovementSystem() override = default;

	virtual void Tick(class World* world, float deltaTime) override;
	
};

