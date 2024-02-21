#pragma once
#include "ECS.h"
#include "Script.h"
#include "Transform.h"
#include "WorldManager.h"
#include "RenderManager.h"

class OutroScript : public Script
{
public:
	OutroScript(Entity* ent)
		: Script(ent)
	{}
	~OutroScript() override = default;

	void Awake() override
	{
		m_pOwner->get<Sprite2D>().get().m_Position[0] = 0;
		m_pOwner->get<Sprite2D>().get().m_Position[1] = 20;
		m_pOwner->get<Sprite2D>().get().m_Layer = 0.0f;
	}

	void Update(float deltaTime) override
	{
		m_ElapsedTime += deltaTime;
		m_Duration += deltaTime;
		
		// �̹����� ������ �����ϸ�
		if (m_pOwner->get<Sprite2D>().get().m_Position[0] <= m_TargetPosition)		// �̹��� ���α��̰� ������ ���� �̹��� ���� ��
		{
			// ���� �ð� ���� �̹����� ���ߵ��� ��
			if (m_ElapsedTime <= m_StopTime)
				return;

			m_pOwner->get<Sprite2D>().get().m_IsRendered = false;

			// ���̵� �ƿ�. ���� ȭ���� ���´�.
			if (m_ElapsedTime <= m_StopTime + 2)
				return;

			m_pOwner->get<Sprite2D>().get().m_Position[0] = 2024;
			// Ÿ��Ʋ ȭ������ �̵�	-> GameApp ���� ���� ���� Title ������ �ٲپ� �ش�.
			return;
		}

		// Ư�� �ð����� ��� ����� �̹����� �̵���Ų��.
		if (m_ElapsedTime < m_PauseTime)
			return;

		m_pOwner->get<Sprite2D>().get().m_IsRendered = true;

		// �ƿ�Ʈ�� �̹����� �̵���Ų��.
		if (m_Duration >= m_fps)
		{
			m_pOwner->get<Sprite2D>().get().m_Position[0] -= m_Step;	// �ڷ����� long
			m_Duration = 0.f;
		}

		m_StopTime = m_ElapsedTime + m_PauseTime; // ���� �ð����� ���� �ð� ����
	}

private:
	float m_ElapsedTime = 0.f;
	float m_fps = 0.0003;		// TODO : ���̹�
	float m_PauseTime = 4.f;
	float m_StopTime = 0.f;
	float m_Duration = 0.f;
	long m_TargetPosition = -14265;	// �̹��� �ʺ� - �ػ� �ʺ�..
	int m_Step = 1;
};

