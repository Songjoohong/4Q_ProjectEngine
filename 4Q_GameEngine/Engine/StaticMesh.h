#pragma once
#include "ECS.h"

struct StaticMesh
{
	ECS_DECLARE_TYPE

	StaticMesh() = default;
	StaticMesh(const std::string& fileName)
		: m_FileName(fileName)
	{}

	std::string m_FileName = "";
};

ECS_DEFINE_TYPE(StaticMesh)