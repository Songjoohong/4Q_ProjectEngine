#pragma once
#include "ECS.h"

struct SkinnedMesh
{
	ECS_DECLARE_TYPE

	SkinnedMesh() = default;
	SkinnedMesh(const char* fileName, const char* animationName)
		: m_FileName(fileName)
		, m_AnimationName(animationName)
	{}

	const char* m_FileName = "";
	const char* m_AnimationName = "";
};


ECS_DEFINE_TYPE(SkinnedMesh)
