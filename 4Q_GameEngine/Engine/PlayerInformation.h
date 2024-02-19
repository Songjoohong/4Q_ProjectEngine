#pragma once
#include "ECS.h"
#include <queue>

struct PlayerInformation
{
	ECS_DECLARE_TYPE
	PlayerInformation() = default;

	std::queue<int> m_VisitedRooms;

	std::string m_ComponentName = "PlayerInformation";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerInformation, m_ComponentName)
};

ECS_DEFINE_TYPE(PlayerInformation)