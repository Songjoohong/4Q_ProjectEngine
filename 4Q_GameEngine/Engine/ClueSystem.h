#pragma once
#include "ECS.h"

using namespace ECS;

class ClueSystem : public EntitySystem,
	public EventSubscriber<Events::CluePageChange>,
	public EventSubscriber<Events::ClueFound>
{
	virtual void Configure(World* world) override;
	virtual void Deconfigure(World* world) override;

	virtual void Receive(World* world, const Events::CluePageChange& event) override;
	virtual void Receive(World* world, const Events::ClueFound& event) override;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;

private:
	bool m_IsClueOn = false;
	int m_CurrentPage = 1;
};

