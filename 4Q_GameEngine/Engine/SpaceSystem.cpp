#include "pch.h"
#include "SpaceSystem.h"

#include "InputManager.h"
#include "Space.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "PlayerScript.h"

void SpaceSystem::Configure(World* world)
{
	world->Subscribe<Events::SpaceAssemble>(this);
}

void SpaceSystem::Deconfigure(World* world)
{
	world->UnsubscribeAll(this);
}

void SpaceSystem::Receive(World* world, const Events::SpaceAssemble& event)
{
	if (!event.objectEntity->has<Space>() || !event.subjectEntity->has<Space>())
		return;
	const auto object = event.objectEntity->get<Space>();
	const auto subject = event.subjectEntity->get<Space>();
	Vector3D subjectDistance = subject->m_Exits[event.subjectExit - 1].m_Distance;
	const Vector3D objectDistance = object->m_Exits[event.objectExit - 1].m_Distance;
	//Vector3D initialDistance = subjectDistance;
	const int rotationKey = object->m_Exits[event.objectExit - 1].m_ExitDirection - subject->m_Exits[event.subjectExit - 1].m_ExitDirection;

	if (rotationKey == -1 || rotationKey == 3)
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(270.f);
		subjectDistance = Vector3D{ -subjectDistance.GetY(), subjectDistance.GetX(),subjectDistance.GetZ() };
	}
	else if (rotationKey == 1 || rotationKey == -3)
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(90.f);
		subjectDistance = Vector3D{ subjectDistance.GetY(), -subjectDistance.GetX(),subjectDistance.GetZ() };
	}
	else if (rotationKey == 0)
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(180.f);
		subjectDistance = Vector3D{ -subjectDistance.GetX(), -subjectDistance.GetY(),subjectDistance.GetZ() };
	}
	else
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(0.f);
	}
	const Vector3D vec = objectDistance - subjectDistance;
	event.subjectEntity->get<Transform>()->m_Position = event.objectEntity->get<Transform>()->m_Position + vec;
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

	vector<Entity*> _entity;
	world->each<Space, BoxCollider>([&](Entity* ent, ComponentHandle<Space> space, ComponentHandle<BoxCollider> collider)->void
		{
			if (collider->m_State == CollisionState::ENTER && space->m_IsPlayerExist)
			{
				space->m_IsPlayerExist = false;
				_entity.push_back(ent);
			}
			else
			{
				space->m_IsPlayerExist = true;
			}
		});

	/*world->each<PlayerScript>([&](Entity* ent_player, ComponentHandle<PlayerScript> player)->void
		{
			if (!_entity.empty())
			{
				for (auto& ent : _entity)
				{
					player->m_VisitedRooms.push(ent->getEntityId());
					cout << "Player visite Room : " << ent->getEntityId() << endl;
				}
			}
		});*/

	_entity.clear();
}
