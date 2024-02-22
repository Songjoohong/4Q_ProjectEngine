#pragma once
#include "ECS.h"

struct Clue
{
	ECS_DECLARE_TYPE
		Clue() = default;
	Clue(int page, int index, std::string path)
		: m_Page(page)
		, m_Index(index)
		, m_FilePath(path)
	{}
	~Clue() = default;

	int m_Page;
	int m_Index;
	std::string m_FilePath;

	std::string m_ComponentName = "Clue";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Clue, m_ComponentName, m_Page, m_Index, m_FilePath)
};
ECS_DEFINE_TYPE(Clue)