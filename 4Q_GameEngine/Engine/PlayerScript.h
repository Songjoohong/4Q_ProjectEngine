#pragma once
#include "DynamicCollider.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class PlayerScript : public Script
{
public:
	PlayerScript(Entity* ent)
		: Script(ent)
	{}
	virtual ~PlayerScript() override = default;

	virtual void Awake() override
	{
		m_pOwner->get<Transform>()->m_FreezeRotationY = true;
		m_pOwner->get<Transform>()->m_FreezeRotationZ = true;

		PhysicsManager::GetInstance()->GetDynamicCollider(m_pOwner->getEntityId())->FreezeRotation(true,true,true);

		Input->SetCameraMode(true);
	}

	virtual void Update(float deltaTime) override
	{
		if(Input->GetKey(Key::UP))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::FRONT;
		}
		if (Input->GetKey(Key::LEFT))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::LEFTWARD;
		}
		if (Input->GetKey(Key::RIGHT))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::RIGHTWARD;
		}
		if (Input->GetKey(Key::DOWN))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::BACK;
		}

		m_pOwner->get<Movement>()->m_CurrentRotation[0] = Input->GetMouseMove().x;
	}
};