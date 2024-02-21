#pragma once
#include "ECS.h"

struct Interactive
{
	ECS_DECLARE_TYPE
	Interactive() = default;
	~Interactive() = default;

	std::string m_ComponentName = "Interactive";
	int m_OpeningDir = 0;
	bool m_IsInteract = false;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Interactive, m_ComponentName, m_OpeningDir)
};

ECS_DEFINE_TYPE(Door)