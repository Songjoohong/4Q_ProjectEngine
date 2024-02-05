#pragma once
#include "ECS.h"

using namespace ECS;

class CameraSystem : public EntitySystem
{
	void Tick(World* world, ECS::DefaultTickData data) override;
};

