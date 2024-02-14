#pragma once
#include "ECS.h"
#include "Vector3D.h"


enum CollisionType
{
	DYNAMIC,
	STATIC,
	PLANE,
};

enum Collision_Mask
{
	PLAYER,
	WALL,
	GROUND,
	SLOPE,
	OBJECT,
	BLOCK
};

struct BoxCollider
{
	ECS_DECLARE_TYPE

	BoxCollider() = default;
	BoxCollider(CollisionType type, Collision_Mask mask,Vector3D scale)
		: m_CollisionType(type)
		, m_CollisionMask(mask)
		, m_Size(scale)
	{}

	std::string m_ComponentName = "BoxCollider";
	CollisionType m_CollisionType;
	Collision_Mask m_CollisionMask;
	Vector3D m_Center = { 0.f,0.f,0.f };
	Vector3D m_Size = { 1.f,1.f,1.f };
	Vector3D m_Rotation = { 0.f,0.f,0.f };
	bool m_IsTrigger = false;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider, m_ComponentName, m_CollisionType, m_CollisionMask, m_Center, m_Size, m_Rotation, m_IsTrigger)
};

ECS_DEFINE_TYPE(BoxCollider)

