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
	float m_StaticFriction = 1.0f;
	float m_DynamicFriction = 1.0f;
	float m_Restitution = 0.0f;
	float m_MaxVellocity = 100.f;
};

ECS_DEFINE_TYPE(RigidBody)