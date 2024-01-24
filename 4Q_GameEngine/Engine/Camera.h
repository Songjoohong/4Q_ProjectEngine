#pragma once
#include "ECS.h"

struct Camera
{
	ECS_DECLARE_TYPE

		Camera() = default;

	float m_FOV = 90.f;
	float m_Near = 1.f;
	float m_Far = 1000.f;
};

ECS_DEFINE_TYPE(Camera)

