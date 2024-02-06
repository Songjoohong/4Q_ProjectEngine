#pragma once
#include "ECS.h"
#include "Vector3D.h"


enum CollisionType
{
	DYNAMIC,
	STATIC,
	PLANE,
};

struct BoxCollider
{
	ECS_DECLARE_TYPE

	BoxCollider() = default;
	BoxCollider(CollisionType type, float a)
		: m_CollisionType(type)
		, m_Size({ a,1,a })
	{}

	std::string m_ComponentName = "BoxCollider";
	CollisionType m_CollisionType;
	Vector3D m_Center = { 0.f,0.f,0.f };
	Vector3D m_Size = { 1.f,1.f,1.f };
	Vector3D m_Rotation = { 0.f,0.f,0.f };
	bool m_IsTrigger = false;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider, m_ComponentName, m_Center, m_Size, m_IsTrigger)
};

ECS_DEFINE_TYPE(BoxCollider)

