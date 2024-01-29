#pragma once
#include "ECS.h"

struct StaticMesh
{
	ECS_DECLARE_TYPE


		StaticMesh() = default;
	explicit StaticMesh(const char* fileName)
		: m_FileName(fileName)
	{}

	const char* m_FileName = "";
};

ECS_DEFINE_TYPE(StaticMesh)

