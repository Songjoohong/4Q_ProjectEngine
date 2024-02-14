#pragma once
#include "ECS.h"

using namespace ECS;

struct DynamicText
{
	ECS_DECLARE_TYPE

	DynamicText() = default;
	explicit DynamicText(const std::vector<std::string>& text, const float correction = 0.f, const float distance = 100.f)
		: m_Text(text)
		, m_TriggerDistance(distance)
		, m_YAxisCorrection(correction)
	{}
	~DynamicText() = default;
	
	std::vector<std::string> m_Text;
	float m_TriggerDistance = 100.f;
	float m_YAxisCorrection = 0.f;
	int m_CurrentTextIndex = 0;
	bool m_IsTextShow = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DynamicText, m_TriggerDistance, m_CurrentTextIndex, m_Text, m_IsTextShow)
};
ECS_DEFINE_TYPE(DynamicText)