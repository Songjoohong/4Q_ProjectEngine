#pragma once
#include "ISingleton.h"

class TimeManager : public ISingleton<TimeManager>
{
public:
	TimeManager() = default;
	~TimeManager() override = default;

public:
	void Initialize();
	void Update();
	void Pause();
	void Resume();

	float GetDeltaTime() const;

private:
	LARGE_INTEGER m_Frequency = {};
	LARGE_INTEGER m_PreviousTime = {};
	LARGE_INTEGER m_CurrentTime = {};
	float m_DeltaTime = 0.f;
	float m_PausedTime = 0.f;
	bool m_IsPaused = false;
};

