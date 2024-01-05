#include "pch.h"
#include "MovementComponent.h"

#include "InputSystem.h"
#include "TransformComponent.h"

void MovementComponent::Update()
{
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>(m_Key[0])).KeyState == KeyState::STAY)
	{
		GetOwner()->FindComponent<TransformComponent>()->MovePosition(Vector3D{ 1.f, 0.f, 0.f } * m_Speed);
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>(m_Key[1])).KeyState == KeyState::STAY)
	{
		GetOwner()->FindComponent<TransformComponent>()->MovePosition(Vector3D{ -1.f, 0.f, 0.f } * m_Speed);
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>(m_Key[2])).KeyState == KeyState::STAY)
	{
		GetOwner()->FindComponent<TransformComponent>()->MovePosition(Vector3D{ 0.f, 1.f, 0.f } * m_Speed);
	}
	if (InputSystem::GetInstance()->GetKeyState(static_cast<Key>(m_Key[3])).KeyState == KeyState::STAY)
	{
		GetOwner()->FindComponent<TransformComponent>()->MovePosition(Vector3D{ 0.f, -1.f, 0.f } * m_Speed);
	}
}
