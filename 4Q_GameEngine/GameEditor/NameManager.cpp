#include "pch.h"
#include "NameManager.h"
#include "../Engine/ECS.h"
#include "../Engine/EntityIdentifier.h"
NameManager::NameManager() : m_NameContainer()
{
}

NameManager::~NameManager()
{
}

void NameManager::AddEntityName(ECS::Entity* entity)
{
	if (entity->has<EntityIdentifier>())
	{
		CheckNameDuplicated(entity);
		m_NameContainer.push_back(entity->get<EntityIdentifier>().get().m_EntityName);
	}
}

void NameManager::CheckNameDuplicated(ECS::Entity* entity)
{
	int overlappedNameCount = 0;

	if (overlappedNameCount != 0)
	{
		entity->get<EntityIdentifier>().get().m_EntityName;
		return;
	}

	for (const auto& entityName : m_NameContainer)
	{
		std::string name;
		if (overlappedNameCount == 0)
		{
			name = entity->get<EntityIdentifier>().get().m_EntityName;
		}
		else
		{
			//if(entity->get<EntityIdentifier>()->m_EntityName.)
			name = entity->get<EntityIdentifier>().get().m_EntityName + " (" + std::to_string(overlappedNameCount) + ")";
		}

		if (entityName == name)
		{
			overlappedNameCount++;
		}
	}
	if (overlappedNameCount == 0)
	{
		entity->get<EntityIdentifier>().get().m_EntityName;
	}
	else
	{
		entity->get<EntityIdentifier>().get().m_EntityName += " (" + std::to_string(overlappedNameCount) + ")";
	}
}

void NameManager::ClearContainer()
{
	m_NameContainer.clear();
}
