#pragma once
#include "ECS.h"

using namespace ECS;

struct DynamicText
{
	ECS_DECLARE_TYPE

	DynamicText() = default;
	explicit DynamicText(const std::vector<std::wstring>& text, const float correction = 0.f, const float distance = 100.f)
		: m_Text(text)
		, m_TriggerDistance(distance)
	{}
	~DynamicText() = default;
	
	std::vector<std::wstring> m_Text;
	float m_TriggerDistance = 100.f;
	int m_CurrentTextIndex = 0;
	bool m_IsTextShow = false;

	std::string m_ComponentName = "DynamicText";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DynamicText, m_ComponentName, m_TriggerDistance, m_CurrentTextIndex, m_Text, m_IsTextShow)
};
ECS_DEFINE_TYPE(DynamicText)