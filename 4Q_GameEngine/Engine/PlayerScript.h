#pragma once
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
	}
};