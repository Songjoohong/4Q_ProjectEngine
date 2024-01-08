#pragma once

#include "ECS.h"
#include "Vector3D.h"

struct Transform
{
	ECS_DECLARE_TYPE

	Transform() = default;
	explicit Transform(const Vector3D xyz_Position, const Vector3D xyz_Rotation = {0.f, 0.f, 0.f})
		: m_Position(xyz_Position)
		, m_Rotation(xyz_Rotation)
	{
	}

	Vector3D m_Position;
	Vector3D m_Rotation;
};

ECS_DEFINE_TYPE(Transform)