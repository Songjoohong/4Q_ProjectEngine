#pragma once

enum PhysicsType
{
	DYNAMIC,
	STATIC,
	PLANE	// ���� : ���� ����
};

enum ObjectType
{
	PLAYER = (1 << 0),
	GROUND = (1 << 1),
	SLOPE = (1 << 2),
	WALL = (1 << 3),
	OBJECT = (1 << 4)
};