#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class FreeCameraScript : public Script
{
public:
	bool m_IsCameraWork = false;
	FreeCameraScript(Entity* ent)
		:Script(ent)
	{
	}
	virtual ~FreeCameraScript() override = default;

	virtual void Update(float deltaTime) override
	{
		if(Input->GetMouseButtonDown(Key::SPACE))
		{
			m_IsCameraWork = !m_IsCameraWork;
			ShowCursor(!m_IsCameraWork);
			Input->SetCameraMode(m_IsCameraWork);
		}

		if(m_IsCameraWork)
		{
			// KeyInput
			if (Input->GetKey(Key::W))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::FRONT;
			if (Input->GetKey(Key::A))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::LEFTWARD;
			if (Input->GetKey(Key::S))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::BACK;
			if (Input->GetKey(Key::D))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::RIGHTWARD;
			if (Input->GetKey(Key::Q))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::UPWARD;
			if (Input->GetKey(Key::E))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::DOWNWARD;

			m_pOwner->get<Movement>()->m_CurrentRotation[0] = Input->GetMouseMove().x;
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = Input->GetMouseMove().y;
		}

	}
};