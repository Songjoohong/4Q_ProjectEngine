#pragma once
#include "ECS.h"

struct Interactive
{
	ECS_DECLARE_TYPE
	Interactive() = default;
	~Interactive() = default;

	std::string m_ComponentName = "Interactive";
	int m_OpeningDir;
};

ECS_DEFINE_TYPE(Door)