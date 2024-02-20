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

		m_TargetPosition = Vector3D{ 0.0f, 190.0f, -400.0f };

		m_AccelerationTime = 0.5f;
	}

	virtual void Update(float deltaTime) override
	{
		if (InputManager::GetInstance()->GetKeyDown(Key::SPACE))
		{
			m_IsCameraMoving = true;
		}
		if (m_IsCameraMoving)
		{
			m_CurrentTime += deltaTime;

			float t = min(m_CurrentTime / m_MoveDuration, 1.0f);

			if (m_CurrentTime < m_AccelerationTime)
			{
				t = 0.5f * std::pow(t / m_AccelerationTime, 2);
			}
			else if (m_CurrentTime > m_MoveDuration - m_AccelerationTime)
			{
				float decelerationT = (m_CurrentTime - (m_MoveDuration - m_AccelerationTime)) / m_AccelerationTime;
				t = 1.0f - 0.5f * std::pow(1.0f - decelerationT, 2);
			}

			Vector3D interpolatedPosition;
			interpolatedPosition.SetX(DirectX::SimpleMath::Vector3::Lerp(m_StartPosition.ConvertToVector3(), m_TargetPosition.ConvertToVector3(), t).x);
			interpolatedPosition.SetY(DirectX::SimpleMath::Vector3::Lerp(m_StartPosition.ConvertToVector3(), m_TargetPosition.ConvertToVector3(), t).y);
			interpolatedPosition.SetZ(DirectX::SimpleMath::Vector3::Lerp(m_StartPosition.ConvertToVector3(), m_TargetPosition.ConvertToVector3(), t).z);

			m_pOwner->get<Transform>()->m_Position = interpolatedPosition;

			if (t >= 1.0f)
			{
				m_CurrentTime = 0.0f;
			}
		}

		if (m_pOwner->get<Transform>()->m_Position == m_TargetPosition)
		{
			m_IsCameraMoving = false;
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