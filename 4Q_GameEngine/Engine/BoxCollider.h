#pragma once
#include "ECS.h"
#include "Vector3D.h"


enum CollisionState
{
	NONE,
	ENTER,
	STAY,
	EXIT
};
struct BoxCollider
{
	ECS_DECLARE_TYPE
		BoxCollider() = default;
		BoxCollider(int parentEntityId) : m_ParentEntityId(parentEntityId) {}

	CollisionState m_CurrentState = CollisionState::NONE;
	Vector3D m_Center = { 0.f,0.f,0.f };
	Vector3D m_Size = { 1.f,1.f,1.f };
	bool m_IsTrigger = false;

	int m_ParentEntityId = 0;
	std::string m_ComponentName = "BoxCollider";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider, m_CurrentState, m_Center, m_Size, m_IsTrigger)
};

ECS_DEFINE_TYPE(BoxCollider)

