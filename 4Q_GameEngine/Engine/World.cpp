#include "pch.h"
#include "World.h"

#include "GameObject.h"

World::~World()
{
	for(const auto& object : m_pGameObjects)
	{
		delete object;
	}
	m_pGameObjects.clear();
}

void World::Update() const
{
	for(const auto& object : m_pGameObjects)
	{
		object->Update();
	}
}
