#pragma once

#include <DirectXMath.h>

#include "ECS.h"
#include "Vector3D.h"

enum MoveState
{
	IDLE = 0,
	FRONT = 0x0001,
	BACK = 0x0010,
	LEFT = 0x0100,
	RIGHT = 0x1000,
	UPWARD = 0x10000,
	DOWNWARD = 0x100000,
};
struct Movement
{
	ECS_DECLARE_TYPE

	Movement() = default;

	std::string m_ComponentName = "Movement";

	float m_Speed = 100.f;
	float m_Sensitivity = 0.1f;
	int m_CurrentMoveState = 0;

	Vector3D m_DirectionVector = { 0.f, 0.f, 1.f };
	Vector3D m_RightVector = { 0.f,0.f,0.f };

	POINT m_CurrentRotation = { 0,0 };

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Movement, m_ComponentName, m_Speed, m_Sensitivity, m_CurrentMoveState, m_DirectionVector, m_RightVector)
};

ECS_DEFINE_TYPE(Movement)

