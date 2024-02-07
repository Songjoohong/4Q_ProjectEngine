#pragma once
#include "ECS.h"

struct RigidBody
{
	ECS_DECLARE_TYPE

	RigidBody() = default;
	~RigidBody() = default;

	float m_Mass = 50.f;
	bool m_IsGravity = false;
	bool m_FreezeRotation[3] = { false, false, false };
};

ECS_DEFINE_TYPE(RigidBody)