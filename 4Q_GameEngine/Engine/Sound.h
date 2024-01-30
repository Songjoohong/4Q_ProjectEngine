#pragma once
#include "ECS.h"

struct Sound
{
	ECS_DECLARE_TYPE

	Sound(const char* fileName, bool isLoop)
		: m_FileName(fileName)
		, m_IsLoop(isLoop)
	{}

	const char* m_FileName;
	float m_Volume = 1.f;
	bool m_IsPlaying = false;
	bool m_IsLoop;
};

ECS_DEFINE_TYPE(Sound)