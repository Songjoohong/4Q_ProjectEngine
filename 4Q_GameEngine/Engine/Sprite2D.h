#pragma once
#include "ECS.h"

struct Sprite2D
{
	Sprite2D() = default;
	explicit Sprite2D(const ECS::Entity* ent, const char* fileName, const float layer, const POINT pos = { 0,0 })
		: m_EntityID(static_cast<int>(ent->getEntityId()))
		, m_FileName(fileName)
		, m_Layer(layer)
		, m_Position{ pos.x, pos.y }
	{}

	int m_EntityID;
	const char* m_FileName;
	// Layer는 0 과 1 사이
	float m_Layer;
	long m_Position[2];
	bool m_IsRendered = true;
};

ECS_DEFINE_TYPE(Sprite2D)