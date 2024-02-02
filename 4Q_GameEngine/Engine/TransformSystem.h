#pragma once
#include "ECS.h"

using namespace ECS;
class TransformSystem : public EntitySystem
{
public:
	TransformSystem() = default;
	virtual ~TransformSystem() = default;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;
};

