#pragma once
#include "ECS.h"
#include "Vector3D.h"

struct BoxCollider
{
	ECS_DECLARE_TYPE

	BoxCollider() = default;

	Vector3D m_Center = { 0.f,0.f,0.f };
	Vector3D m_Size = { 1.f,1.f,1.f };
	bool m_IsTrigger = false;
};

ECS_DEFINE_TYPE(BoxCollider)