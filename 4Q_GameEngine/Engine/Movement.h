#pragma once

#include "ECS.h"
#include "Vector3D.h"


struct Movement
{
	ECS_DECLARE_TYPE
		Movement() = default;

	float m_Speed = 1.f;
	Vector3D m_DirectionVector = { 0.f, 0.f, 0.f };

	std::string m_ComponentName = "Movement";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Movement, m_Speed, m_DirectionVector)
};

ECS_DEFINE_TYPE(Movement)

