#pragma once

#include "ECS.h"
struct EntityIdentifier
{
	ECS_DECLARE_TYPE
		EntityIdentifier() = default;
	explicit EntityIdentifier(int entityId, std::string entityName, int parentEntityId = 0)
		:m_EntityId(entityId), m_ParentEntityId(parentEntityId), m_EntityName(entityName)
	{
	}

	std::string m_EntityName = "Empty Entity";
	int			m_EntityId = 0;
	std::string m_ComponentName = "EntityIdentifier";
	bool		m_HasParent = false;
	int			m_ParentEntityId = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(EntityIdentifier, m_ComponentName, m_EntityName, m_EntityId, m_ParentEntityId, m_HasParent)
};

ECS_DEFINE_TYPE(EntityIdentifier)