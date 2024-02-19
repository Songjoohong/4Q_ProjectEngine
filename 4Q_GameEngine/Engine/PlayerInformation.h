#pragma once
#include "ECS.h"
#include <queue>

struct PlayerInformation
{
	ECS_DECLARE_TYPE
	PlayerInformation() = default;

	std::string m_ComponentName = "PlayerInformation";
	std::queue<int> m_VisitedRooms;
	std::string m_LookingEntity = "";
	std::vector<string> m_CollidingEntity;
};

ECS_DEFINE_TYPE(PlayerInformation)