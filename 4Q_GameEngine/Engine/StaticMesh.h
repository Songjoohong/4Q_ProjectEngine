#pragma once
#include "ECS.h"


struct StaticMesh
{
	ECS_DECLARE_TYPE

	StaticMesh() = default;
	StaticMesh(int entityId, const std::string& fileName)
		: m_EntityId(entityId), m_FileName(fileName)
	{}

	std::string m_FileName = "";

	int m_EntityId = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(StaticMesh, m_EntityId, m_FileName)
};

ECS_DEFINE_TYPE(StaticMesh)

