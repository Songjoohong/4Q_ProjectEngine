#pragma once
#include "ECS.h"
#include "Vector3D.h"

struct ExitInfo
{
	int m_ExitDirection = 0;
	Vector3D m_Distance;
};

struct Space
{
	ECS_DECLARE_TYPE
	Space() = default;
	Space(int index, std::vector<ExitInfo> exit)
		: m_SpaceIndex(index)
		, m_Exits(exit)
	{}
	~Space() = default;

	int m_SpaceIndex;
	std::vector<ExitInfo> m_Exits;
	bool m_IsPlayerExist = false;
};
ECS_DEFINE_TYPE(Space)