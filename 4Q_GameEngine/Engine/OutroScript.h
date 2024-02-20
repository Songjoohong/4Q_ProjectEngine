#pragma once
#include "ECS.h"
#include "Script.h"
#include "Transform.h"
#include "WorldManager.h"

class OutroScript : public Script
{
public:
	OutroScript(Entity* ent)
		: Script(ent)
	{}
	~OutroScript() override = default;

	void Update(float deltaTime) override
	{
		// for debug
		// Ư�� ��ġ�� �����ϸ�
		if (m_pOwner->get<Transform>()->m_Position.m_X >= 100)
		{
			// ���̵� �ƿ�. ���� ȭ���� ���´�.
			// Ÿ��Ʋ ȭ������ �̵�	-> GameApp ���� ���� ���� Title ������ �ٲپ� �ش�.
			return;
		}

		m_ElapsedTime += deltaTime;

		// Ư�� �ð����� ��� ����� ī�޶� �̵���Ų��.
		if (m_ElapsedTime < m_PauseTime)
			return;

		m_pOwner->get<Transform>()->m_Position.AddX(deltaTime * m_Speed);

		// TODO: ������������ �ε巴�� ��ġ�̵�.
		//m_pOwner->get<Transform>()->m_Position.SetX();
	}

private:
	float m_ElapsedTime = 0.f;
	float m_Speed = 10.f;
	float m_PauseTime = 5.f;
};