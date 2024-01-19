#pragma once
#include "Vector3D.h"
#include "ECS.h"

namespace Component
{
	enum LightType
	{
		Directional,
		Point,
	};

	struct Light
	{
		ECS_DECLARE_TYPE

			LightType m_Type = LightType::Directional;
		Vector3D m_Color = { 1.f,1.f,1.f };
		float m_Intensity = 1.f;

	};

	ECS_DEFINE_TYPE(Light)
}
