#pragma once
#include "ECS.h"


struct StaticMesh
{
	ECS_DECLARE_TYPE

	StaticMesh() = default;
	StaticMesh(const std::string& fileName)
		: m_FileName(fileName)
	{}

	std::string m_ComponentName = "StaticMesh";
	std::string m_FileName = "";
	bool m_IsModelCreated = false;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(StaticMesh, m_ComponentName, m_FileName, m_IsModelCreated)
};

ECS_DEFINE_TYPE(StaticMesh)

