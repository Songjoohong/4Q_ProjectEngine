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
	
	std::vector<std::wstring> m_Text; // TODO imgui 한글 출력 오류로 대기중
	float m_TriggerDistance = 100.f; // TODO 완료.
	int m_CurrentTextIndex = 0;	// TODO.. 이건 나타내지 않고 조정할 수 있도록 해서 에디터에 나타내는 m_Text 값 변경하게하기
	bool m_IsTextShow = false;

	std::string m_ComponentName = "DynamicText";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DynamicText, m_ComponentName, m_TriggerDistance, m_CurrentTextIndex, m_Text, m_IsTextShow)
};
ECS_DEFINE_TYPE(DynamicText)