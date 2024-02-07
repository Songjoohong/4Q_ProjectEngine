#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class CameraScript : public Script
{
public:
	CameraScript(Entity* ent)
		:Script(ent)
	{
	}
	virtual ~CameraScript() override = default;

	virtual void Update(float deltaTime) override
	{
		// KeyInput
		if (Input->GetKey(Key::W))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::FRONT;
		if (Input->GetKey(Key::A))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::LEFT;
		if (Input->GetKey(Key::S))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::BACK;
		if (Input->GetKey(Key::D))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::RIGHT;
		if (Input->GetKey(Key::Q))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::UPWARD;
		if (Input->GetKey(Key::E))
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::DOWNWARD;



		m_pOwner->get<Movement>()->m_CurrentRotation[0] = Input->GetMouseMove().x;
		m_pOwner->get<Movement>()->m_CurrentRotation[1] = Input->GetMouseMove().y;
	}
};