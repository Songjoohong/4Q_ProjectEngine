#pragma once
#include "ECS.h"

struct PlayerInformation;
using namespace ECS;
class EventSystem : public EntitySystem
{
public:
	EventSystem() = default;
	virtual ~EventSystem() = default;

	virtual void Tick(World* world, ECS::DefaultTickData data) override;

	static bool CheckRoom(ComponentHandle<PlayerInformation> info, int spaceIndex);
	static bool IsColliding(ComponentHandle<PlayerInformation> info, std::string entityName);
	static bool IsLooking(ComponentHandle<PlayerInformation> info, std::string entityName);
	int m_CurrentEventIndex = 0;
	bool m_Trigger[20] = { false, };
	
};

