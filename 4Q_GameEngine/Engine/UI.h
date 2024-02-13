#pragma once
#include "ECS.h"

enum UISTATE
{
	HOVER,
	CLICK,
	UINONE,
};
struct UI
{
	ECS_DECLARE_TYPE

	UI() = default;
	UI(int width, int height)
		: m_Size{width, height}
	{}
	~UI() = default;

	long m_Size[2] = { 0,0 };
	UISTATE m_UIstate = UISTATE::UINONE;
};
ECS_DEFINE_TYPE(UI);