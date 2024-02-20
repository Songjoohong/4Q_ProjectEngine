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
		// 특정 위치에 도달하면
		if (m_pOwner->get<Transform>()->m_Position.m_X >= 100)
		{
			// 페이드 아웃. 검은 화면이 나온다.
			// 타이틀 화면으로 이동	-> GameApp 에서 현재 씬을 Title 씬으로 바꾸어 준다.
			return;
		}

		m_ElapsedTime += deltaTime;

		// 특정 시간동안 잠시 멈췄다 카메라를 이동시킨다.
		if (m_ElapsedTime < m_PauseTime)
			return;

		m_pOwner->get<Transform>()->m_Position.AddX(deltaTime * m_Speed);

		// TODO: 선형보간으로 부드럽게 위치이동.
		//m_pOwner->get<Transform>()->m_Position.SetX();
	}

private:
	float m_ElapsedTime = 0.f;
	float m_Speed = 10.f;
	float m_PauseTime = 5.f;
};