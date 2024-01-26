#pragma once
#include "ECS.h"

namespace Component
{
	struct SkinnedMesh
	{
		ECS_DECLARE_TYPE

<<<<<<< Updated upstream
			SkinnedMesh() = default;
		SkinnedMesh(const std::string& fileName, const std::string& animationName)
			: m_FileName(fileName)
			, m_AnimationName(animationName)
		{}

		std::string m_FileName = "";
		std::string m_AnimationName = "";
	};
=======
		SkinnedMesh() = default;
	SkinnedMesh(const char* fileName, const char* animationName)
		: m_FileName(fileName)
		, m_AnimationName(animationName)
	{}

	const char* m_FileName = "";
	const char* m_AnimationName = "";
};

ECS_DEFINE_TYPE(SkinnedMesh)
>>>>>>> Stashed changes

	ECS_DEFINE_TYPE(SkinnedMesh)
}
