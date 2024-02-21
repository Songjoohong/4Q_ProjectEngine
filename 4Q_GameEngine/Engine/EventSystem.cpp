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
			if(IsColliding(info, "room_08_triggercoll_01"))
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
			if (IsColliding(info, "room_08_triggercoll_02"))
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
			if (IsColliding(info, "room_08_triggercoll_04"))
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

			if (IsColliding(info, "room_09_triggercoll_02") && m_Trigger[4] && m_CurrentEventIndex != 51)
			{
				m_CurrentEventIndex = 51;
				m_Trigger[4] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,2,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_09_triggercoll_03") && m_CurrentEventIndex != 9)
			{
				m_Trigger[5] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_04") && m_Trigger[5] && m_CurrentEventIndex != 52)
			{
				m_CurrentEventIndex = 52;
				m_Trigger[5] = false;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,2,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_09_triggercoll_05") && IsLooking(info, "room_07_wall_04_wall") && m_CurrentEventIndex != 53)
			{
				m_CurrentEventIndex = 53;
				world->emit<Events::SpaceReturn>({ 37 });
				world->emit<Events::SpaceAssemble>({ 7,29,1,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_09_triggercoll_02") && m_CurrentEventIndex != 11)
			{
				m_Trigger[6] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_01") && m_Trigger[6] && m_CurrentEventIndex != 54)
			{
				m_CurrentEventIndex = 54;
				m_Trigger[6] = false;
				world->emit<Events::SpaceReturn>({ 29 });

				// meal 관련 추가
			}

			//이벤트 E
			if (IsColliding(info, "room_09_triggercoll_04") && m_CurrentEventIndex != 12)
			{
				m_Trigger[7] = true;
			}

			if (IsColliding(info, "room_09_triggercoll_03") && m_Trigger[7] && m_CurrentEventIndex != 55)
			{
				m_CurrentEventIndex = 55;
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

			//이벤트 C
			if(m_CurrentEventIndex != 56 && false)
			{
				m_CurrentEventIndex = 56;
			}
			
		}

		if(CheckRoom(info, 13))
		{
			//이벤트 A
			if(IsColliding(info, "room_13_triggercoll_01") && IsLooking(info, "room_13_obj_scroll_01_01") && m_CurrentEventIndex != 15)
			{
				m_CurrentEventIndex = 15;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 B
			if(IsColliding(info, "room_13_triggercoll_02") && IsLooking(info, "room_13_obj_scroll_01_02") && m_CurrentEventIndex != 16)
			{
				m_CurrentEventIndex = 16;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_13_triggercoll_03") && IsLooking(info, "room_13_obj_scroll_01_03") && m_CurrentEventIndex != 17)
			{
				m_CurrentEventIndex = 17;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_13_triggercoll_04") && IsLooking(info, "room_13_obj_scroll_01_04") && m_CurrentEventIndex != 18)
			{
				m_CurrentEventIndex = 18;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 13,18,5,0 });
			}

			//이벤트 E
			if(IsColliding(info, "room_13_triggercoll_05") && m_CurrentEventIndex != 19)
			{
				m_CurrentEventIndex = 19;
				world->emit<Events::SpaceAssemble>({ 13,14,6,1 });
				world->emit<Events::SpaceAssemble>({ 14,7,0,0 });
			}
		}

		if (CheckRoom(info, 14))
		{
			if(IsColliding(info, "room_14_triggercoll_01") && m_CurrentEventIndex != 20)
			{
				m_CurrentEventIndex = 20;
				world->emit<Events::SpaceReturn>({ 14 });
				world->emit<Events::SpaceAssemble>({ 7,6,0,0 });
			}
		}

		if(CheckRoom(info, 15))
		{
			//이벤트 A
			if(IsColliding(info, "room_15_triggercoll_01") && IsLooking(info, "room_15_obj_scroll_02_01") && m_CurrentEventIndex != 21)
			{
				m_CurrentEventIndex = 21;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_15_triggercoll_02") && IsLooking(info, "room_15_obj_scroll_03_01") && m_CurrentEventIndex != 22)
			{
				m_CurrentEventIndex = 22;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_15_triggercoll_03") && IsLooking(info, "room_15_obj_scroll_03_02") && m_CurrentEventIndex != 23)
			{
				m_CurrentEventIndex = 23;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_15_triggercoll_04") && IsLooking(info, "room_15_obj_scroll_04_01") && m_CurrentEventIndex != 24)
			{
				m_CurrentEventIndex = 24;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceAssemble>({ 13,18,5,0 });
			}

			//이벤트 E
			if (IsColliding(info, "room_15_triggercoll_05") && IsLooking(info, "room_15_obj_scroll_04_02") && m_CurrentEventIndex != 25)
			{
				m_CurrentEventIndex = 25;
				world->emit<Events::SpaceReturn>({ 15 });
				world->emit<Events::SpaceAssemble>({ 13,18,5,0 });
			}
		}

		if(CheckRoom(info, 16))
		{
			//이벤트 A
			if (IsColliding(info, "room_16_triggercoll_01") && IsLooking(info, "room_16_obj_scroll_01_01") && m_CurrentEventIndex != 26)
			{
				m_CurrentEventIndex = 26;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_16_triggercoll_02") && IsLooking(info, "room_16_obj_scroll_03_01") && m_CurrentEventIndex != 27)
			{
				m_CurrentEventIndex = 27;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_16_triggercoll_03") && IsLooking(info, "room_16_obj_scroll_03_02") && m_CurrentEventIndex != 28)
			{
				m_CurrentEventIndex = 28;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_16_triggercoll_04") && IsLooking(info, "room_16_obj_scroll_03_03") && m_CurrentEventIndex != 29)
			{
				m_CurrentEventIndex = 29;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}

			//이벤트 E
			if (IsColliding(info, "room_16_triggercoll_05") && IsLooking(info, "room_16_obj_scroll_04_01") && m_CurrentEventIndex != 30)
			{
				m_CurrentEventIndex = 30;
				world->emit<Events::SpaceReturn>({ 16 });
				world->emit<Events::SpaceAssemble>({ 13,18,5,0 });
			}
		}

		if(CheckRoom(info, 17))
		{
			//이벤트 A
			if (IsColliding(info, "room_17_triggercoll_01") && IsLooking(info, "room_17_obj_scroll_01_01") && m_CurrentEventIndex != 31)
			{
				m_CurrentEventIndex = 31;
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_17_triggercoll_02") && IsLooking(info, "room_17_obj_scroll_01_02") && m_CurrentEventIndex != 32)
			{
				m_CurrentEventIndex = 32;
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_17_triggercoll_03") && IsLooking(info, "room_17_obj_scroll_02_01") && m_CurrentEventIndex != 33)
			{
				m_CurrentEventIndex = 33;
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_17_triggercoll_04") && IsLooking(info, "room_17_obj_scroll_02_02") && m_CurrentEventIndex != 34)
			{
				m_CurrentEventIndex = 34;
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}
		}

		if(CheckRoom(info, 18))
		{
			//이벤트 A
			if (IsColliding(info, "room_18_triggercoll_01") && IsLooking(info, "room_18_obj_scroll_01_01") && m_CurrentEventIndex != 35)
			{
				m_CurrentEventIndex = 35;
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,15,5,0 });
			}

			//이벤트 B
			if (IsColliding(info, "room_18_triggercoll_02") && IsLooking(info, "room_18_obj_scroll_02_01") && m_CurrentEventIndex != 36)
			{
				m_CurrentEventIndex = 36;
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}

			//이벤트 C
			if (IsColliding(info, "room_18_triggercoll_03") && IsLooking(info, "room_18_obj_scroll_02_02") && m_CurrentEventIndex != 37)
			{
				m_CurrentEventIndex = 37;
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,16,5,0 });
			}

			//이벤트 D
			if (IsColliding(info, "room_18_triggercoll_04") && IsLooking(info, "room_18_obj_scroll_03_01") && m_CurrentEventIndex != 38)
			{
				m_CurrentEventIndex = 38;
				world->emit<Events::SpaceReturn>({ 18 });
				world->emit<Events::SpaceAssemble>({ 13,17,5,0 });
			}
		}

		if(CheckRoom(info, 19))
		{
			if(IsColliding(info, "room_19_triggercoll_01") && m_CurrentEventIndex != 39)
			{
				m_CurrentEventIndex = 39;
				world->emit<Events::SpaceAssemble>({ 19,20,1,0 });
				world->emit<Events::SpaceAssemble>({ 20,28,1,0 });
			}
		}

		if(CheckRoom(info, 20))
		{
			if (IsColliding(info, "room_20_triggercoll_01") && IsLooking(info, "room_20_obj_01") && m_CurrentEventIndex != 50)
			{
				m_CurrentEventIndex = 50;
				world->emit<Events::SpaceReturn>({ 19 });
				world->emit<Events::SpaceReturn>({ 17 });
				world->emit<Events::SpaceAssemble>({ 20,35,0,0 });
			}
		}

		if (CheckRoom(info, 28))
		{
			if(IsColliding(info, "miniRoom_01_triggercoll_01") && (IsLooking(info, "miniRoom_01_obj_01") || IsLooking(info, "miniRoom_01_obj_02")) && m_CurrentEventIndex != 40)
			{
				m_CurrentEventIndex = 40;
				world->emit<Events::SpaceReturn>({ 20 });
				world->emit<Events::SpaceAssemble>({ 28,21,0,1 });
				world->emit<Events::SpaceAssemble>({ 21,35,0,0 });
				world->emit<Events::SpaceAssemble>({ 21,22,2,0 });
				world->emit<Events::SpaceAssemble>({ 22,30,1,0 });
				world->emit<Events::SpaceAssemble>({ 22,29,2,0 });
			}
		}

		if(CheckRoom(info,22))
		{
			//이벤트 A
			if (IsColliding(info, "room_22_triggercoll_01") && (IsLooking(info, "room_22_wall_02") || IsLooking(info, "room_22_wall_03") || IsLooking(info, "room_22_wall_04") || IsLooking(info, "room_22_wall_05") || IsLooking(info, "room_22_wall_06")) && m_CurrentEventIndex != 41)
			{
				m_CurrentEventIndex = 41;
				world->emit<Events::SpaceReturn>({ 28 });
				world->emit<Events::SpaceReturn>({ 21 });
				world->emit<Events::SpaceAssemble>({ 22,35,0,0 });
			}

			//이벤트 B
			if(IsColliding(info, "room_22_triggercoll_01"))
			{
				m_Trigger[8] = true;
				if (m_Trigger[9])
					m_Trigger[10] = true;
			}

			if(IsColliding(info, "miniRoom_02_triggercoll_01") && m_Trigger[8])
			{
				m_Trigger[9] = true;
				if(m_Trigger[10] && m_CurrentEventIndex != 42)
				{
					m_CurrentEventIndex = 42;
					world->emit<Events::SpaceReturn>({ 22 });
					world->emit<Events::SpaceAssemble>({ 30,23,0,1 });
					world->emit<Events::SpaceAssemble>({ 23,35,0,0 });
					world->emit<Events::SpaceAssemble>({ 23,29,2,0 });
					world->emit<Events::SpaceAssemble>({ 23,24,3,0 });
					world->emit<Events::SpaceAssemble>({ 24,30,1,0 });
					world->emit<Events::SpaceAssemble>({ 24,31,2,0 });
					world->emit<Events::SpaceAssemble>({ 24,34,3,0 });
				}
			}
		}

		if(CheckRoom(info, 24))
		{
			//이벤트 A
			if(IsColliding(info, "room_24_triggercoll_01") && m_CurrentEventIndex != 43)
			{
				m_Trigger[11] = true;
				if(IsLooking(info, "room_24_wall_06") && m_CurrentEventIndex != 43)
				{
					m_CurrentEventIndex = 43;
					world->emit<Events::SpaceReturn>({ 23 });
					world->emit<Events::SpaceReturn>({ 28 });
					world->emit<Events::SpaceReturn>({ 29 });
					world->emit<Events::SpaceAssemble>({ 24,35,0,0 });
				}				
			}

			//이벤트 B
			if (IsColliding(info, "miniRoom_03_triggercoll_01") && m_Trigger[11] && (IsLooking(info, "miniRoom_01_obj_01 (1)") || IsLooking(info, "miniRoom_01_obj_02 (1)")) && m_CurrentEventIndex != 44)
			{
				m_CurrentEventIndex = 44;
				m_Trigger[11] = false;
				world->emit<Events::SpaceReturn>({ 30 });
				world->emit<Events::SpaceReturn>({ 31 });
				world->emit<Events::SpaceReturn>({ 34 });
				world->emit<Events::SpaceAssemble>({ 24,28,1,0 });
				world->emit<Events::SpaceAssemble>({ 24,29,2,0 });
				world->emit<Events::SpaceAssemble>({ 24,31,3,0 });
			}

			//이벤트 C
			if (IsColliding(info, "miniRoom_02_triggercoll_01") && m_Trigger[11] && (IsLooking(info, "miniRoom_02_obj_01") || IsLooking(info, "miniRoom_02_obj_02")) && m_CurrentEventIndex != 45)
			{
				m_CurrentEventIndex = 45;
				m_Trigger[11] = false;
				world->emit<Events::SpaceReturn>({ 28 });
				world->emit<Events::SpaceReturn>({ 29 });
				world->emit<Events::SpaceReturn>({ 31 });
				world->emit<Events::SpaceAssemble>({ 24,31,1,0 });
				world->emit<Events::SpaceAssemble>({ 24,34,2,0 });
				world->emit<Events::SpaceAssemble>({ 24,32,3,0 });
			}

			//이벤트 D
			if (IsColliding(info, "miniRoom_05_triggercoll_01") && m_Trigger[11] && (IsLooking(info, "miniRoom_01_obj_01 (3)") || IsLooking(info, "miniRoom_01_obj_02 (3)")) && m_CurrentEventIndex != 46)
			{
				m_CurrentEventIndex = 46;
				m_Trigger[11] = false;
				world->emit<Events::SpaceReturn>({ 31 });
				world->emit<Events::SpaceReturn>({ 34 });
				world->emit<Events::SpaceReturn>({ 32 });
				world->emit<Events::SpaceAssemble>({ 24,28,1,0 });
				world->emit<Events::SpaceAssemble>({ 24,33,2,0 });
				world->emit<Events::SpaceAssemble>({ 24,31,3,0 });
			}

			//이벤트 E
			if (IsColliding(info, "miniRoom_06_triggercoll_01") && m_Trigger[11] && (IsLooking(info, "miniRoom_02_obj_01 (2)") || IsLooking(info, "miniRoom_02_obj_02 (1)") || IsLooking(info, "miniRoom_02_obj_03")) && m_CurrentEventIndex != 47)
			{
				m_CurrentEventIndex = 47;
				m_Trigger[11] = false;
				world->emit<Events::SpaceReturn>({ 24 });
				world->emit<Events::SpaceAssemble>({ 29,25,0,2 });
				world->emit<Events::SpaceAssemble>({ 25,35,0,0 });
				world->emit<Events::SpaceAssemble>({ 25,28,1,0 });
				world->emit<Events::SpaceAssemble>({ 25,31,3,0 });
				world->emit<Events::SpaceAssemble>({ 25,39,4,0 });
				world->emit<Events::SpaceAssemble>({ 39,7,1,0 });
			}
		}

		if(CheckRoom(info,39))
		{
			if(IsColliding(info, "room_25_01_triggercoll_01") && (IsLooking(info, "room_07_wall_08_2ndwall") || IsLooking(info, "room_07_wall_08_ceiling") || IsLooking(info, "room_07_wall_08_wall")) && m_CurrentEventIndex != 48)
			{
				m_CurrentEventIndex = 48;
				world->emit<Events::SpaceReturn>({ 25 });
				world->emit<Events::SpaceReturn>({ 39 });
				world->emit<Events::SpaceAssemble>({ 7,6,0,0 });
			}
		}

		if(CheckRoom(info, 29))
		{
			if(IsColliding(info, "room_29_triggercoll_01") && IsLooking(info, "room_10_wall_07 (1)") && m_CurrentEventIndex != 49)
			{
				m_CurrentEventIndex = 49;
				world->emit<Events::SpaceReturn>({ 7 });
				world->emit<Events::SpaceAssemble>({ 29,28,0,0 });
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
