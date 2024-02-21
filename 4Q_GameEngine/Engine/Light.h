#pragma once
#include "Vector3D.h"
#include "ECS.h"


enum LightType
{
	Directional,
	Point,
};

struct Light
{
	ECS_DECLARE_TYPE
		Light() = default;

	std::string m_ComponentName = "Light";

	LightType m_Type = LightType::Directional;
	Vector3D m_Color = { 1.f,1.f,1.f };
	Vector3D m_Direction = {0.f, -1.f, 1.f};
	float m_Intensity = 1.f;
	float m_Radius;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Light, m_ComponentName, m_Type, m_Color, m_Intensity)
};

ECS_DEFINE_TYPE(Light)

