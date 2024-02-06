#pragma once
#include "ECS.h"

struct Sound
{
	ECS_DECLARE_TYPE

		Sound() = default;
	Sound(std::string fileName, bool isLoop)
		: m_FileName(fileName)
		, m_IsLoop(isLoop)
	{}

	std::string m_ComponentName = "Sound";

	std::string m_FileName;
	float m_Volume = 1.f;
	bool m_IsPlaying = false;
	bool m_IsLoop;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sound, m_ComponentName, m_FileName, m_Volume, m_IsPlaying, m_IsLoop)
};

ECS_DEFINE_TYPE(Sound)