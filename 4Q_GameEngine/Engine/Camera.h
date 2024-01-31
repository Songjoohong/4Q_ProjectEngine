#pragma once
#include "ECS.h"

struct Camera
{
	ECS_DECLARE_TYPE
		Camera() = default;
		Camera(int entityId) : m_EntityId(entityId) {}

	float m_FOV = 90.f;
	float m_Near = 1.f;
	float m_Far = 1000.f;

	int m_EntityId = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Camera, m_EntityId, m_FOV, m_Near, m_Far)
};

ECS_DEFINE_TYPE(Camera)

