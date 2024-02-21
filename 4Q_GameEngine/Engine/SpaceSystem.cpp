#include "pch.h"
#include "SpaceSystem.h"

#include "InputManager.h"
#include "Space.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "PlayerInformation.h"

void SpaceSystem::Configure(World* world)
{
	world->Subscribe<Events::SpaceAssemble>(this);
	world->Subscribe<Events::SpaceReturn>(this);
}

void SpaceSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void SpaceSystem::Receive(World* world, const Events::SpaceAssemble& event)
{
	Entity* objectEntity;
	Entity* subjectEntity;

	world->each<Space>([&](Entity* ent, ComponentHandle<Space> space)->void
		{
			if (space->m_SpaceIndex == event.objectIndex)
				objectEntity = ent;
			else if (space->m_SpaceIndex == event.subjectIndex)
				subjectEntity = ent;

		});
	const auto object = objectEntity->get<Space>();
	const auto subject = subjectEntity->get<Space>();
	Vector3D& subjectDistance = subject->m_Exits[event.subjectExit].m_Distance;
	const Vector3D objectDistance = object->m_Exits[event.objectExit].m_Distance;
	//Vector3D initialDistance = subjectDistance;
	const int rotationKey = object->m_Exits[event.objectExit].m_ExitDirection - subject->m_Exits[event.subjectExit].m_ExitDirection;

	if (rotationKey == -1 || rotationKey == 3)
	{
		subjectEntity->get<Transform>()->m_Rotation.SetY(270.f);
		subjectDistance = Vector3D{ -subjectDistance.GetZ(), subjectDistance.GetY(),subjectDistance.GetX() };
		subject->m_Exits[event.subjectExit].m_ExitDirection = (subject->m_Exits[event.subjectExit].m_ExitDirection + 1) % 4;
	}
	else if (rotationKey == 1 || rotationKey == -3)
	{
		subjectEntity->get<Transform>()->m_Rotation.SetY(90.f);
		subjectDistance = Vector3D{ subjectDistance.GetZ(), subjectDistance.GetY(),-subjectDistance.GetX() };
		subject->m_Exits[event.subjectExit].m_ExitDirection = (subject->m_Exits[event.subjectExit].m_ExitDirection + 3) % 4;
	}
	else if (rotationKey == 0)
	{
		subjectEntity->get<Transform>()->m_Rotation.SetY(180.f);
		subjectDistance = Vector3D{ -subjectDistance.GetX(), subjectDistance.GetY(), -subjectDistance.GetZ() };
		subject->m_Exits[event.subjectExit].m_ExitDirection = (subject->m_Exits[event.subjectExit].m_ExitDirection + 2) % 4;
	}
	else
	{
		subjectEntity->get<Transform>()->m_Rotation.SetY(0.f);
	}
	const Vector3D vec = objectDistance - subjectDistance;
	subjectEntity->get<Transform>()->m_Position.SetY(0);
	subjectEntity->get<Transform>()->m_Position = objectEntity->get<Transform>()->m_Position + vec;
}

void SpaceSystem::Receive(World* world, const Events::SpaceReturn& event)
{
	world->each<Space, Transform>([&](Entity* ent, ComponentHandle<Space> space, ComponentHandle<Transform> transform)
		{
			if (event.spaceIndex == space->m_SpaceIndex)
			{
				transform->m_Position.SetY(event.spaceIndex * 10000.f);
			}
		});
}


void SpaceSystem::Tick(World* world, ECS::DefaultTickData data)
{
	//return all
	if (InputM->GetKeyDown(Key::F12))
	{
		world->each<Space, Transform>([&](Entity* ent, ComponentHandle<Space> space, ComponentHandle<Transform> transform)->void
			{
				if (!space->m_IsPlayerExist)
					transform->m_Position.SetY(10000.f * space->m_SpaceIndex);
			});

	}

	std::vector<Entity*> _entity;
	world->each<Space, BoxCollider>([&](Entity* ent, ComponentHandle<Space> space, ComponentHandle<BoxCollider> collider)->void
		{
			if (collider->m_State == CollisionState::ENTER)
			{
				if (space->m_IsPlayerExist == true)
				{
					space->m_IsPlayerExist = false;
					_entity.push_back(ent);
				}
				else
				{
					collider->m_State = CollisionState::EXIT;
					space->m_IsPlayerExist = true;
				}
			}
			else if (collider->m_State == CollisionState::EXIT)
			{
				space->m_IsPlayerExist = true;
			}
		});

	world->each<PlayerInformation>([&](Entity* ent_player, ComponentHandle<PlayerInformation> component)->void
		{
			if (!_entity.empty())
			{
				for (auto& ent : _entity)
				{
					component->m_VisitedRooms.push(ent->get<Space>()->m_SpaceIndex);
					std::cout << "Player visit Room : " << ent->get<Space>()->m_SpaceIndex << std::endl;
				}
			}

		});

	_entity.clear();
}

