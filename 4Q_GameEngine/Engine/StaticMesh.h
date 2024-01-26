#pragma once
#include "ECS.h"

namespace Component
{
	struct StaticMesh
	{
		ECS_DECLARE_TYPE

<<<<<<< Updated upstream
			StaticMesh() = default;
		StaticMesh(const std::string& fileName)
			: m_FileName(fileName)
		{}

		std::string m_FileName = "";
	};
=======
	StaticMesh() = default;
	explicit StaticMesh(const char* fileName)
		: m_FileName(fileName)
	{}

	const char* m_FileName = "";
};

ECS_DEFINE_TYPE(StaticMesh)
>>>>>>> Stashed changes

	ECS_DEFINE_TYPE(StaticMesh)
}
