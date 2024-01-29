#pragma once
#include "ECS.h"

#define GetFSM GetOwner()->get<FSM>()

namespace ECS
{
	class Entity;
}

struct State
{
	State(ECS::Entity* ent)
		: m_pOwner(ent)
	{}
	virtual ~State() = default;

	virtual void Enter() {}
	virtual void Execute() {}
	virtual void Exit() {}

	void SetName(const char* name) { m_Name = name; }
	[[nodiscard]]const char* GetName() const { return m_Name; }
	[[nodiscard]] ECS::Entity* GetOwner() const { return m_pOwner; }
private:
	const char* m_Name = "";
	ECS::Entity* m_pOwner = nullptr;
};

