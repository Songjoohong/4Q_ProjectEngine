#pragma once

#include "ECS.h"
#include "Vector3D.h"

struct Movement
{
	float m_Speed = 1.f;
	Vector3D m_DirectionVector = { 0.f, 0.f, 0.f };
};
