#pragma once
#include "ECS.h"
#include "ISingleton.h"

class WorldManager : public ISingleton<WorldManager>
{
public:
	WorldManager() = default;
	virtual ~WorldManager() override = default;

private:
	ECS::World* m_CurrentWorld = nullptr;

public:
	[[nodiscard]]ECS::World* GetCurrentWorld() const { return m_CurrentWorld; }

	void Update(const float deltaTime) const { m_CurrentWorld->Tick(deltaTime); }
	void ChangeWorld(ECS::World* world);
};

