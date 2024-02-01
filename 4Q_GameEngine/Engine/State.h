#pragma once
#include "ECS.h"

#define GetFSM GetOwner()->get<FSM>()

namespace ECS
{
	class Entity;
}

class State
{
public:
	explicit State(ECS::Entity* ent)
		: m_pOwner(ent)
	{}
	virtual ~State() = default;

	virtual void Enter() {}
	virtual void Execute() {}
	virtual void Exit() {}

	void SetName(std::string name) { m_Name = name; }
	[[nodiscard]] std::string GetName() const { return m_Name; }
	[[nodiscard]] ECS::Entity* GetOwner() const { return m_pOwner; }

	//NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, m_Name)
private:
	std::string m_Name = "";
	ECS::Entity* m_pOwner = nullptr;

};

