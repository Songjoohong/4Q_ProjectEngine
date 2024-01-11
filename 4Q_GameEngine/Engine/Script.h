#pragma once

using namespace ECS;
class Script
{
public:
	Script(ECS::Entity* owner)
		: m_pOwner(owner)
	{}
	virtual ~Script() = default;

private:
	Entity* m_pOwner = nullptr;

public:
	ECS::Entity* GetOwner() const { return m_pOwner; }

	virtual void Update() {}
	virtual void Awake() {}
};
