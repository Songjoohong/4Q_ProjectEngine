#pragma once
#include "ECS.h"
#include "Vector3D.h"
struct Sprite2D
{
	Sprite2D() = default;
	explicit Sprite2D(const ECS::Entity* ent, std::string fileName, const float layer, const MyPoint pos = {0,0 })
		: m_FileName(fileName)
		, m_Layer(layer)
		, m_Position(pos)
	{}

	std::string m_FileName;
	float m_Layer;
	MyPoint m_Position;
	bool m_IsRendered = true;
	std::string m_ComponentName = "Sprite2D";

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sprite2D, m_FileName, m_Layer, m_Position, m_IsRendered)
};

ECS_DEFINE_TYPE(Sprite2D)