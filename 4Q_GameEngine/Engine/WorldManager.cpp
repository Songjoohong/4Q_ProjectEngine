#include "pch.h"
#include "WorldManager.h"

void WorldManager::ChangeWorld(ECS::World* world)
{
	/*if(m_CurrentWorld != nullptr)
		m_CurrentWorld->DestroyWorld();*/
	m_CurrentWorld = world;
}
