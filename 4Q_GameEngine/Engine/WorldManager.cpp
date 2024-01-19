#include "pch.h"
#include "WorldManager.h"

void WorldManager::ChangeWorld(ECS::World* world)
{
	m_CurrentWorld->DestroyWorld();
	m_CurrentWorld = world;
}
