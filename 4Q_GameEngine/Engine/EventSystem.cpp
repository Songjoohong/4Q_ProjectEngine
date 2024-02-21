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
		if (CheckRoom(info, 2))
		{
			if ((IsLooking(info, "room_02_floor_coll_04") || IsLooking(info, "room_02_floor_coll_03") || IsLooking(info, "room_02_floor_coll_02")) && IsColliding(info, "room_02_triggercoll_01") && m_CurrentEventIndex != 1)
			{
				m_CurrentEventIndex = 1;
				world->emit<Events::SpaceReturn>({ 1 });
				world->emit<Events::SpaceReturn>({ 4 });
				world->emit<Events::SpaceReturn>({ 5 });
				world->emit<Events::SpaceAssemble>({ 2,3,0,1 });
				world->emit<Events::SpaceAssemble>({ 3,6,0,0 });
			}
		}

		if (CheckRoom(info, 3))
		{
			if (IsColliding(info, "room_03_triggercoll_01") && IsLooking(info, "room_03_door_01") && m_CurrentEventIndex != 2)
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

		if (CheckRoom(info, 6))
		{
			if (IsColliding(info, "room_06_triggercoll_01") && IsLooking(info, "room_03_door_01") && m_CurrentEventIndex != 4)
			{
				m_CurrentEventIndex = 4;
				world->emit<Events::SpaceReturn>({ 3 });
				world->emit<Events::SpaceAssemble>({ 6,7,0,0 });
			}
		}

		if(CheckRoom(info, 7))
		{
			if(IsColliding(info, "room_07_triggercoll_01") && IsLooking(info, "room_07_wall_08")  && m_CurrentEventIndex != 5)
			{
				m_CurrentEventIndex = 5;
				world->emit<Events::SpaceAssemble>({ 7,8,1,0 });
			}

			if(IsColliding(info, "room_07_triggercoll_01") && m_CurrentEventIndex != 6)
			{
				m_CurrentEventIndex = 6;
				world->emit<Events::SpaceAssemble>({ 7,19,3,0 });
			}

			if(false/*7번 이벤트 C*/)
			{
				m_CurrentEventIndex = 7;
				world->emit<Events::SpaceAssemble>({ 7,9,2,0 });
			}
		}

		if (CheckRoom(info, 8))
		{
			//이벤트 A
			if(IsColliding(info, "room_08_triggercoll_01") && m_CurrentEventIndex != 8)
			{
				m_Trigger[0] = true;
			}

			if (IsColliding(info, "room_08_triggercoll_02") && m_Trigger[0] && m_CurrentEventIndex != 8)
			{
				m_CurrentEventIndex = 8;
				m_Trigger[0] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,10,1,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_08_triggercoll_03") && m_CurrentEventIndex != 9)
			{
				m_Trigger[1] = true;
			}

			if (IsColliding(info, "room_08_triggercoll_04") && m_Trigger[1] && m_CurrentEventIndex != 9)
			{
				m_CurrentEventIndex = 9;
				m_Trigger[1] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,10,1,0 });
			}

			//이벤트 C
			if(IsColliding(info, "room_08_triggercoll_05") && IsLooking(info, "room_07_wall_05_wall") && m_CurrentEventIndex != 10)
			{
				m_CurrentEventIndex = 10;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,10,1,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_08_triggercoll_02") && m_CurrentEventIndex != 11)
			{
				m_Trigger[2] = true;
			}

			if (IsColliding(info, "room_08_triggercoll_01") && m_Trigger[2] && m_CurrentEventIndex != 11)
			{
				m_CurrentEventIndex = 11;
				m_Trigger[2] = false;
				world->emit<Events::SpaceReturn>({ 10 });
				
				// meal 관련 추가
			}

			//이벤트 E
			if (IsColliding(info, "room_08_triggercoll_04") && m_CurrentEventIndex != 12)
			{
				m_Trigger[3] = true;
			}

			if (IsColliding(info, "room_08_triggercoll_03") && m_Trigger[3] && m_CurrentEventIndex != 12)
			{
				m_CurrentEventIndex = 12;
				m_Trigger[3] = false;
				world->emit<Events::SpaceReturn>({ 10 });
				// meal 관련 추가
			}
		}

		if (CheckRoom(info, 9))
		{
			//이벤트 A
			if (IsColliding(info, "room_09_triggercoll_01") && m_CurrentEventIndex != 8)
			{
				m_Trigger[4] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_02") && m_Trigger[4] && m_CurrentEventIndex != 8)
			{
				m_CurrentEventIndex = 8;
				m_Trigger[4] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,2,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_09_triggercoll_03") && m_CurrentEventIndex != 9)
			{
				m_Trigger[5] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_04") && m_Trigger[5] && m_CurrentEventIndex != 9)
			{
				m_CurrentEventIndex = 9;
				m_Trigger[5] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,2,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_09_triggercoll_05") && IsLooking(info, "room_07_wall_04_wall") && m_CurrentEventIndex != 10)
			{
				m_CurrentEventIndex = 10;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,1,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_09_triggercoll_02") && m_CurrentEventIndex != 11)
			{
				m_Trigger[6] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_01") && m_Trigger[6] && m_CurrentEventIndex != 11)
			{
				m_CurrentEventIndex = 11;
				m_Trigger[6] = false;
				world->emit<Events::SpaceReturn>({ 29 });

				// meal 관련 추가
			}

			//이벤트 E
			if (IsColliding(info, "room_09_triggercoll_04") && m_CurrentEventIndex != 12)
			{
				m_Trigger[7] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_03") && m_Trigger[7] && m_CurrentEventIndex != 12)
			{
				m_CurrentEventIndex = 12;
				m_Trigger[7] = false;
				world->emit<Events::SpaceReturn>({ 29 });
				// meal 관련 추가
			}
		}

		if (CheckRoom(info, 10))
		{
			//이벤트 A
			if(IsColliding(info, "room_10_triggercoll_01") && IsLooking(info, "room_10-wall_07") && m_CurrentEventIndex != 13)
			{
				m_CurrentEventIndex = 13;
				world->emit<Events::SpaceReturn>({ 7 });
				world->emit<Events::SpaceReturn>({ 8 });
				world->emit<Events::SpaceReturn>({ 9 });
				world->emit<Events::SpaceReturn>({ 19 });
				world->emit<Events::SpaceAssemble>({ 10,38,0,0 });
				world->emit<Events::SpaceAssemble>({ 10,13,0,1 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 B //check
			if(false && m_CurrentEventIndex != 14)
			{
				m_CurrentEventIndex = 14;
				world->emit<Events::SpaceReturn>({ 13 });
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 10,13,0,0 });
				world->emit<Events::SpaceAssemble>({ 13,36,1,0 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}


		}
	}
	

	

}

bool EventSystem::CheckRoom(ComponentHandle<PlayerInformation> info, int spaceIndex)
{
	return info->m_VisitedRooms.size() && info->m_VisitedRooms.back() == spaceIndex;
}

bool EventSystem::IsColliding(ComponentHandle<PlayerInformation> info, std::string entityName)
{
	return std::find(info->m_CollidingEntities.begin(), info->m_CollidingEntities.end(), "room_10_triggercoll_01") != info->m_CollidingEntities.end();
}

bool EventSystem::IsLooking(ComponentHandle<PlayerInformation> info, std::string entityName)
{
	return info->m_LookingEntity == entityName;
}
