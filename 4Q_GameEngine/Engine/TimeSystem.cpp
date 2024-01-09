#include "pch.h"
#include "TimeSystem.h"

void TimeSystem::Initialize()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_CurrentTime);
	m_PreviousTime = m_CurrentTime;
}

void TimeSystem::Update()
{
	m_PreviousTime = m_CurrentTime;
	QueryPerformanceCounter(&m_CurrentTime);

	m_DeltaTime = (m_CurrentTime.QuadPart - m_PreviousTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	if (m_IsPaused)
	{
		m_PausedTime += m_DeltaTime;
		m_DeltaTime = 0.f;
	}
}

void TimeSystem::Pause()
{
	if (m_IsPaused == false)
	{
		m_IsPaused = true;
	}

	std::cout << "Paused!" << std::endl;
}

void TimeSystem::Resume()
{
	if (m_IsPaused == true)
	{
		m_IsPaused = false;
		m_PausedTime = 0;
	}
	std::cout << "Resumed!" << std::endl;
}


float TimeSystem::GetDeltaTime() const
{
	return m_DeltaTime;
}
