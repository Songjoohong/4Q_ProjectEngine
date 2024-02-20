#include "pch.h"
#include "EventSystem.h"

#include "PlayerInformation.h"

void EventSystem::Tick(World* world, ECS::DefaultTickData data)
{
	Entity* player;
	ComponentHandle<PlayerInformation> info;
	world->each<PlayerInformation>([&](Entity* ent, ComponentHandle<PlayerInformation> playerInfo)->void
		{
			player = ent;
			info = playerInfo;
		});

	if (info && player)
	{
		if (info->m_VisitedRooms.size() && info->m_VisitedRooms.back() == 2 )
		{
			if ((info->m_LookingEntity == "room_02_floor_coll_04" || info->m_LookingEntity == "room_02_floor_coll_03" || info->m_LookingEntity == "room_02_floor_coll_02") && std::find(info->m_CollidingEntities.begin(), info->m_CollidingEntities.end(), "room_02_triggercoll_01") != info->m_CollidingEntities.end() && m_CurrentEventIndex != 1)
			{
				m_CurrentEventIndex = 1;
				world->emit<Events::SpaceReturn>({ 1 });
				world->emit<Events::SpaceReturn>({ 4 });
				world->emit<Events::SpaceReturn>({ 5 });
				world->emit<Events::SpaceAssemble>({ 2,0,3,0 });
				world->emit<Events::SpaceAssemble>({ 3,0,6,0 });
			}
		}

		if (info->m_VisitedRooms.size() && info->m_VisitedRooms.back() == 3)
		{
			if (std::find(info->m_CollidingEntities.begin(), info->m_CollidingEntities.end(), "room_03_triggercoll_01") != info->m_CollidingEntities.end() && info->m_LookingEntity == "room_03_door_01" && m_CurrentEventIndex != 2)
			{
				m_CurrentEventIndex = 2;
				world->emit<Events::SpaceReturn>({ 2 });
				world->emit<Events::SpaceAssemble>({ 3,4,1,0 });
				world->emit<Events::SpaceAssemble>({ 4,2,1,0 });
			}

			if (info->m_InteractingCount == 2 && m_CurrentEventIndex != 3)
			{
				m_CurrentEventIndex = 3;
				info->m_InteractingCount = 0;
				world->emit<Events::SpaceReturn>({ 6 });
				world->emit<Events::SpaceAssemble>({ 3,5,1,0 });
				world->emit<Events::SpaceAssemble>({ 5,3,0,0 });
			}
		}

		if (info->m_VisitedRooms.size() && info->m_VisitedRooms.back() == 6)
		{
			if (std::find(info->m_CollidingEntities.begin(), info->m_CollidingEntities.end(), "room_06_triggercoll_01") != info->m_CollidingEntities.end() && info->m_LookingEntity == "room_03_door_01" && m_CurrentEventIndex != 4)
			{
				m_CurrentEventIndex = 4;
				world->emit<Events::SpaceReturn>({ 3 });
				world->emit<Events::SpaceAssemble>({ 6,7,0,0 });
			}
		}
	}
	

	

}
