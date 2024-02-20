#pragma once
#include "ECS.h"

struct Door
{
	ECS_DECLARE_TYPE
	Door() = default;
	~Door() = default;

	std::string m_ComponentName = "Door";
	int m_OpeningDir;
};

ECS_DEFINE_TYPE(Door)