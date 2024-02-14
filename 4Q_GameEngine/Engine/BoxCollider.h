#pragma once
#include "ECS.h"
#include "Vector3D.h"


enum ColliderType
{
	DYNAMIC,
	STATIC,
	PLANE,
};

enum Collision_Mask
{
	PLAYER =(1<<0),
	WALL = (2 << 0),
	GROUND = (3 << 0),
	OBJECT = (4 << 0),
	TRIGGER = (5 << 0),
};

struct BoxCollider
{
	ECS_DECLARE_TYPE

	BoxCollider() = default;
	BoxCollider(ColliderType type,Collision_Mask collisionmask, Vector3D scale)
		: m_ColliderType(type)
		, m_CollisionType(collisionmask)
		, m_Size(scale)
	{}

	std::string m_ComponentName = "BoxCollider";
	ColliderType m_ColliderType;
	Collision_Mask m_CollisionType;
	Vector3D m_Center = { 0.f,0.f,0.f };
	Vector3D m_Size = { 1.f,1.f,1.f };
	Vector3D m_Rotation = { 0.f,0.f,0.f };
	bool m_IsTrigger = false;


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider, m_ComponentName, m_ColliderType,m_CollisionType, m_Center, m_Size, m_IsTrigger)
};

ECS_DEFINE_TYPE(BoxCollider)

