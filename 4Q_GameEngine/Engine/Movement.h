#pragma once

#include "ECS.h"
#include "Vector3D.h"

struct Movement
{
	ECS_DECLARE_TYPE

	float m_Speed = 1.f;
	Vector3D m_DirectionVector = { 0.f, 0.f, 0.f };
};

ECS_DEFINE_TYPE(Movement)