#include "pch.h"
#include "SpaceSystem.h"

#include "Space.h"
#include "Transform.h"

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

	if(rotationKey == -1 || rotationKey == 3)
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(270.f);
		subjectDistance = Vector3D{ -subjectDistance.GetY(), subjectDistance.GetX(),subjectDistance.GetZ() };
	}
	else if(rotationKey == 1 || rotationKey == -3)
	{
		event.subjectEntity->get<Transform>()->m_Rotation.SetY(90.f);
		subjectDistance = Vector3D{ subjectDistance.GetY(), -subjectDistance.GetX(),subjectDistance.GetZ() };
	}
	else if(rotationKey == 0)
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

