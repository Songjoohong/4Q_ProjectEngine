#pragma once
#include "ECS.h"

struct Animation
{
	ECS_DECLARE_TYPE

	Animation() {}

	const char* m_AnimationName = "";

	unsigned int m_AnimationIndex = 0;
	unsigned int m_ProgressTime = 0;
	bool m_IsRepeat = false;
};

ECS_DEFINE_TYPE(Animation)
