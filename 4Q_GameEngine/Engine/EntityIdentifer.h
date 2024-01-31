#pragma once

#include "ECS.h"
struct EntityIdentifer
{
	ECS_DECLARE_TYPE
		EntityIdentifer() = default;
	explicit EntityIdentifer(int entityId, std::string entityName, int parentEntityId = 0)
		:m_EntityId(entityId), m_ParentEntityId(parentEntityId), m_EntityName(entityName)
	{
	}


	std::string m_EntityName = " ";
	int			m_EntityId = 0;
	int			m_ParentEntityId = 0;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(EntityIdentifer, m_EntityId, m_EntityName)
};

ECS_DEFINE_TYPE(EntityIdentifer)