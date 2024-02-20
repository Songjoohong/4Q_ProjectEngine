#pragma once
#include "Camera.h"
#include "ECS.h"
#include "InputManager.h"
#include "Movement.h"
#include "Script.h"
#include "Transform.h"
class IntroCameraScript : public Script
{
public:

	explicit IntroCameraScript(Entity* ent)
		:Script(ent)
	{
	}

	virtual ~IntroCameraScript() override = default;

	virtual void Awake() override
	{
		if (m_pOwner->has<Transform>())
			m_StartPosition = m_pOwner->get<Transform>()->m_Position;

		m_TargetPosition = Vector3D{ 0.0f, 190.0f, -550.0f };

		m_MoveDuration = 2.0f;
		m_AccelerationTime = 1.0f;
		m_CurrentTime = 0.0f;
	}

	virtual void Update(float deltaTime) override
	{
		if (InputManager::GetInstance()->GetKeyDown(Key::SPACE) && !m_IsCameraMoving)
		{
			m_IsCameraMoving = true;
		}
		if (m_IsCameraMoving)
		{
			m_CurrentTime += deltaTime;

			float t = 0.0f;
			if (m_CurrentTime < m_AccelerationTime)
			{
				t = 0.5f * std::pow(m_CurrentTime / m_AccelerationTime, 2);
			}
			else if (m_CurrentTime > m_MoveDuration - m_AccelerationTime)
			{
				float decelerationT = (m_CurrentTime - (m_MoveDuration - m_AccelerationTime)) / m_AccelerationTime;
				t = 1.0f - 0.5f * std::pow(1.0f - decelerationT, 2);
			}
			else
			{
				t = (m_CurrentTime - m_AccelerationTime) / (m_MoveDuration - 2 * m_AccelerationTime) + 0.5f;
			}

			float speed = 5.0f;
			Vector3D interpolatedPosition;
			interpolatedPosition = DirectX::SimpleMath::Vector3::Lerp(m_StartPosition.ConvertToVector3(), m_TargetPosition.ConvertToVector3(), t);

			m_pOwner->get<Transform>()->m_Position = interpolatedPosition;

			if (m_CurrentTime >= m_MoveDuration)
			{
				m_IsCameraMoving = false;
				m_CurrentTime = m_MoveDuration;
			}
			
		}
	}

private:
	Vector3D m_StartPosition; 
	Vector3D m_TargetPosition; 
	float m_MoveDuration; 
	float m_CurrentTime;
	float m_AccelerationTime;

	bool m_IsCameraMoving = false;
};