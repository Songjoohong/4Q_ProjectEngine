#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class FreeCameraScript : public Script
{
public:
	bool m_IsCurCameraWork = false;
	bool m_IsPrevCameraWork = false;
	FreeCameraScript(Entity* ent)
		:Script(ent)
	{
	}
	virtual ~FreeCameraScript() override = default;

	virtual void Update(float deltaTime) override
	{
		m_IsPrevCameraWork = m_IsCurCameraWork;
		if (InputM->GetMouseButton(Key::RBUTTON))
			m_IsCurCameraWork = true;
		else
			m_IsCurCameraWork = false;

		if (m_IsCurCameraWork != m_IsPrevCameraWork)
		{
			InputM->SetCameraMode(m_IsCurCameraWork);
			ShowCursor(m_IsCurCameraWork);
		}

		if(m_IsCurCameraWork)
		{
			// KeyInput
			if (InputM->GetKey(Key::W))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::FRONT;
			if (InputM->GetKey(Key::A))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::LEFTWARD;
			if (InputM->GetKey(Key::S))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::BACK;
			if (InputM->GetKey(Key::D))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::RIGHTWARD;
			if (InputM->GetKey(Key::Q))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::UPWARD;
			if (InputM->GetKey(Key::E))
				m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::DOWNWARD;

			m_pOwner->get<Movement>()->m_CurrentRotation[0] = InputM->GetMouseMove().x;
			m_pOwner->get<Movement>()->m_CurrentRotation[1] = InputM->GetMouseMove().y;
		}

	}
};