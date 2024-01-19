#pragma once
#include "ECS.h"

namespace Component
{
	struct SkinnedMesh
	{
		ECS_DECLARE_TYPE

			SkinnedMesh() = default;
		SkinnedMesh(const std::string& fileName, const std::string& animationName)
			: m_FileName(fileName)
			, m_AnimationName(animationName)
		{}

		std::string m_FileName = "";
		std::string m_AnimationName = "";
	};

	ECS_DEFINE_TYPE(SkinnedMesh)
}
