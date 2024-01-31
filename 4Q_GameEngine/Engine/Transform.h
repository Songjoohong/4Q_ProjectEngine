#pragma once

#include "ECS.h"
#include "Vector3D.h"

struct Transform
{
	ECS_DECLARE_TYPE

		Transform() = default;
	explicit Transform(int entityId, const Vector3D xyz_Position, int parentEntityId = 0, const Vector3D xyz_Rotation = { 0.f, 0.f, 0.f }, const Vector3D xyz_Scale = { 1.f, 1.f, 1.f })
		: m_EntityId(entityId)
		, m_ParentEntityId(parentEntityId)
		, m_Position(xyz_Position)
		, m_Rotation(xyz_Rotation)
		, m_Scale(xyz_Scale)
	{
	}

	Vector3D m_Position;
	Vector3D m_Rotation;
	Vector3D m_Scale;

	int m_EntityId = 0;
	int m_ParentEntityId = 0;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, m_EntityId, m_ParentEntityId, m_Position, m_Rotation, m_Scale)
};

ECS_DEFINE_TYPE(Transform)

