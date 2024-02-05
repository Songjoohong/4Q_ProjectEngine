#pragma once
#include "ECS.h"

struct Camera
{
	ECS_DECLARE_TYPE
		Camera() = default;

	std::string m_ComponentName = "Camera";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Camera, m_ComponentName)
};

ECS_DEFINE_TYPE(Camera)

