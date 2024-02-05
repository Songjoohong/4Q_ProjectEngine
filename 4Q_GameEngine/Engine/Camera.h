#pragma once
#include "ECS.h"

struct Camera
{
	ECS_DECLARE_TYPE
		Camera() = default;

	std::string m_ComponentName = "Camera";

	float m_FOV = 90.f;
	float m_Near = 1.f;
	float m_Far = 1000.f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Camera, m_ComponentName, m_FOV, m_Near, m_Far)
};

ECS_DEFINE_TYPE(Camera)

