#pragma once
#include "ECS.h"
#include <queue>

struct PlayerInformation
{
	ECS_DECLARE_TYPE
	PlayerInformation() = default;

	std::vector<std::string> m_CollidingEntities;
	std::queue<int> m_VisitedRooms;
	std::string m_LookingEntity;
	std::string m_ComponentName = "PlayerInformation";
	int m_InteractingCount = 0;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerInformation, m_ComponentName)
};

ECS_DEFINE_TYPE(PlayerInformation)