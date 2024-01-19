#pragma once
#include "ECS.h"

namespace Component
{
	struct Animation
	{
		ECS_DECLARE_TYPE

			Animation() {}

		std::string m_AnimationName = ""; // POD À§¹Ý

		unsigned int m_AnimationIndex = 0;
		unsigned int m_ProgressTime = 0;
		bool m_IsRepeat = false;
	};

	ECS_DEFINE_TYPE(Animation)
}