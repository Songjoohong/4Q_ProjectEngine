#pragma once
#include "ECS.h"
#include "Vector3D.h"

struct ExitInfo
{
	int m_ExitDirection = 0;
	Vector3D m_Distance;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ExitInfo, m_ExitDirection, m_Distance)
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

	int m_SpaceIndex; // TODO
	std::vector<ExitInfo> m_Exits;	// TODO 값 변경 또는 넣는 것도
	bool m_IsPlayerExist = false;

	std::string m_ComponentName = "Space";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Space, m_ComponentName, m_SpaceIndex, m_Exits, m_IsPlayerExist)

};
ECS_DEFINE_TYPE(Space)