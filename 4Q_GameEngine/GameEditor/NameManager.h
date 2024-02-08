#pragma once
#include "Test.h"

namespace ECS { class Entity; }
namespace ECS { class World; }
class NameManager
{
public:
	NameManager();
	~NameManager();


	void AddEntityName(ECS::Entity* entity);
	void CheckNameDuplicated(ECS::Entity* entity);
	std::vector<std::string> m_NameContainer;
};

