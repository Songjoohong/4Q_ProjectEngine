#pragma once
#include "ECS.h"
#include <queue>

struct PlayerInformation
{
	ECS_DECLARE_TYPE
	PlayerInformation() = default;

	std::queue<int> m_VisitedRooms;
};

ECS_DEFINE_TYPE(PlayerInformation)