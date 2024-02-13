#pragma once
#include "ECS.h"
#include "Vector3D.h"
struct Sprite2D
{
	ECS_DECLARE_TYPE
	Sprite2D() = default;
	explicit Sprite2D(const ECS::Entity* ent, std::string fileName, const float layer,long x, long y)
		: m_FileName(fileName)
		, m_Layer(layer)
		, m_Position{ x, y }
	{}

	std::string m_FileName;
	// Layer는 0 과 1 사이
	float m_Layer;
	long m_Position[2];
	bool m_IsRendered = true;
	std::string m_ComponentName = "Sprite2D";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sprite2D, m_FileName, m_Layer, m_Position, m_IsRendered)
};

ECS_DEFINE_TYPE(Sprite2D)