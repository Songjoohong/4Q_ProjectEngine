#pragma once
#include "ECS.h"

struct Debug
{
	ECS_DECLARE_TYPE
	Debug() = default;

	std::string m_ComponentName = "Debug";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Debug, m_ComponentName)
};

ECS_DEFINE_TYPE(Debug)