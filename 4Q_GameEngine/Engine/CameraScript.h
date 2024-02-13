#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"

class CameraScript : public Script
{
	ECS_DECLARE_TYPE
public:
	CameraScript() {}
	CameraScript(Entity* ent)
		:Script(ent)
	{
	}
	virtual ~CameraScript() override = default;

	//CameraScript& operator=(const json& componentData)
	//{
	//	// Copy data from the JSON object to the class members
	//	m_ComponentName = componentData.value("m_ComponentName", "");
	//	// You need to handle other members similarly based on your JSON structure

	//	return *this;
	//}

	virtual void Update(float deltaTime) override
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

		
		m_pOwner->get<Movement>()->m_CurrentRotation[0] = InputM->GetMouseMove().x;
		m_pOwner->get<Movement>()->m_CurrentRotation[1] = InputM->GetMouseMove().y;
	}

	std::string m_ComponentName = "CameraScript";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CameraScript, m_ComponentName)
};

ECS_DEFINE_TYPE(CameraScript)