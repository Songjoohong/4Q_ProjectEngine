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
};
struct Movement
{
	ECS_DECLARE_TYPE

	Movement() = default;

	float m_Speed = 100.f;
	float m_Sensitivity = 1.f;
	int m_CurrentMoveState = 0;

	Vector3D m_DirectionVector = { 0.f, 0.f, 1.f };
	Vector3D m_RightVector = { 0.f,0.f,0.f };

	POINT m_CurrentRotation = { 0,0 };
};

ECS_DEFINE_TYPE(Movement)

