#pragma once
#include "ECS.h"
#include "Vector3D.h"
struct Sprite2D
{
	ECS_DECLARE_TYPE
	Sprite2D() = default;

	explicit Sprite2D(std::string fileName, const float layer = 0 , long x = 0, long y = 0)
		: m_FileName(fileName)
		, m_Layer(layer)
		, m_Position{ x, y }
	{}

	std::string m_FileName;
	// Layer´Â 0 °ú 1 »çÀÌ
	float m_Layer;	// TODO
	long m_Position[2];	// TODO
	bool m_IsRendered = true;
	std::string m_ComponentName = "Sprite2D";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sprite2D, m_ComponentName, m_FileName, m_Layer, m_Position, m_IsRendered)
};

ECS_DEFINE_TYPE(Sprite2D)