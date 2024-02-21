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
		
		// 이미지의 끝까지 도달하면
		if (m_pOwner->get<Sprite2D>().get().m_Position[0] <= m_TargetPosition)		// 이미지 가로길이가 김으로 인해 이미지 가로 ㄱ
		{
			// 일정 시간 동안 이미지를 멈추도록 함
			if (m_ElapsedTime <= m_StopTime)
				return;

			m_pOwner->get<Sprite2D>().get().m_IsRendered = false;

			// 페이드 아웃. 검은 화면이 나온다.
			if (m_ElapsedTime <= m_StopTime + 2)
				return;

			m_pOwner->get<Sprite2D>().get().m_Position[0] = 2024;
			// 타이틀 화면으로 이동	-> GameApp 에서 현재 씬을 Title 씬으로 바꾸어 준다.
			return;
		}

		// 특정 시간동안 잠시 멈췄다 이미지를 이동시킨다.
		if (m_ElapsedTime < m_PauseTime)
			return;

		m_pOwner->get<Sprite2D>().get().m_IsRendered = true;

		// 아웃트로 이미지를 이동시킨다.
		if (m_Duration >= m_fps)
		{
			m_pOwner->get<Sprite2D>().get().m_Position[0] -= m_Step;	// 자료형이 long
			m_Duration = 0.f;
		}

		m_StopTime = m_ElapsedTime + m_PauseTime; // 일정 시간동안 멈출 시간 설정
	}

private:
	float m_ElapsedTime = 0.f;
	float m_fps = 0.0003;		// TODO : 네이밍
	float m_PauseTime = 4.f;
	float m_StopTime = 0.f;
	float m_Duration = 0.f;
	long m_TargetPosition = -14265;	// 이미지 너비 - 해상도 너비..
	int m_Step = 1;
};

