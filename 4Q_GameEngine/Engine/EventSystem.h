#pragma once
#include "ECS.h"

using namespace ECS;
class EventSystem : public EntitySystem
{
public:
	EventSystem() = default;
	virtual ~EventSystem() = default;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;

	int m_CurrentEventIndex = 0;
};

