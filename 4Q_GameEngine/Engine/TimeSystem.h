#pragma once

class TimeSystem
{
public:
	TimeSystem();
	~TimeSystem() = default;

	TimeSystem(const TimeSystem&) = delete;
	void operator=(const TimeSystem&) = delete;
	TimeSystem(const TimeSystem&&) = delete;
	void operator=(const TimeSystem&&) = delete;

private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PreviousTime;
	LARGE_INTEGER m_CurrentTime;
	float m_DeltaTime = 0.f;
	float m_PausedTime = 0.f;
	bool m_IsPaused = false;

public:
	void Update();
	void Pause();
	void Resume();

	float GetDeltaTime() const;

};

