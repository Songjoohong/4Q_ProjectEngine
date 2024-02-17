#pragma once
#include "DynamicCollider.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "Movement.h"
#include "Script.h"
#include "Transform.h"
#include "Space.h"

#include <queue>

class PlayerScript : public Script
{
public:
	PlayerScript(Entity* ent)
		: Script(ent)
	{}
	virtual ~PlayerScript() override = default;
	queue<int> m_VisitedRooms;

	virtual void Awake() override
	{
		m_pOwner->get<Transform>()->m_FreezeRotationX = true;
		m_pOwner->get<Transform>()->m_FreezeRotationZ = true;

		//Todo ¼®¿µ
		//PhysicsManager::GetInstance()->GetDynamicCollider(m_pOwner->getEntityId())->FreezeRotation(true,true,true);

		InputM->SetCameraMode(true);

		//m_pOwner->getWorld()->emit<Events::BroadCastPlayer>({ m_pOwner });
	}

	virtual void Update(float deltaTime) override
	{
		if(InputM->GetKey(Key::UP))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::FRONT;
		}
		if (InputM->GetKey(Key::LEFT))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::LEFTWARD;
		}
		if (InputM->GetKey(Key::RIGHT))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::RIGHTWARD;
		}
		if (InputM->GetKey(Key::DOWN))
		{
			m_pOwner->get<Movement>()->m_CurrentMoveState += MoveState::BACK;
		}

		
		m_pOwner->get<Movement>()->m_CurrentRotation[0] = InputM->GetMouseMove().x;
	}
};