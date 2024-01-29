#pragma once
#include "ECS.h"

struct Animation
{
	ECS_DECLARE_TYPE

	Animation() = default;

	std::string m_AnimationName = ""; // POD À§¹Ý

	unsigned int m_AnimationIndex = 0;
	unsigned int m_ProgressTime = 0;
	bool m_IsRepeat = false;
};

ECS_DEFINE_TYPE(Animation)
