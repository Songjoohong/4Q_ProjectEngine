#include "pch.h"
#include "TimeManager.h"

void TimeManager::Initialize()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_CurrentTime);
	m_PreviousTime = m_CurrentTime;
}

void TimeManager::Update()
{
	m_PreviousTime = m_CurrentTime;
	QueryPerformanceCounter(&m_CurrentTime);

	m_DeltaTime = static_cast<float>(m_CurrentTime.QuadPart - m_PreviousTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	m_ElapsedTime += m_DeltaTime;
	if(m_ElapsedTime >= 1.f)
	{
		m_FPS = 1.f / m_DeltaTime;
		m_ElapsedTime = 0.f;
	}

	if (m_IsPaused)
	{
		m_PausedTime += m_DeltaTime;
		m_DeltaTime = 0.f;
	}
}

void TimeManager::Pause()
{
	if (m_IsPaused == false)
	{
		m_IsPaused = true;
	}

	std::cout << "Paused!" << std::endl;
}

void TimeManager::Resume()
{
	if (m_IsPaused == true)
	{
		m_IsPaused = false;
		m_PausedTime = 0;
	}
	std::cout << "Resumed!" << std::endl;
}


float TimeManager::GetDeltaTime() const
{
	return m_DeltaTime;
}

float TimeManager::GetFPS() const
{
	return m_FPS;
}
