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
	bool m_FreezeLinear[3] = { false, false, false };
	float m_StaticFriction = 1.0f;
	float m_DynamicFriction = 1.0f;
	float m_Restitution = 0.0f;
	float m_MaxVellocity = 100.f;

	std::string m_ComponentName = "RigidBody";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(RigidBody, m_ComponentName, m_Mass, m_IsGravity, m_FreezeRotation, m_FreezeLinear, m_StaticFriction, m_DynamicFriction, m_Restitution, m_MaxVellocity)
};

ECS_DEFINE_TYPE(RigidBody)