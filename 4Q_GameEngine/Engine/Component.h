#pragma once
#include "GameObject.h"
#include "nlohmann/json.hpp"

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

private:
	GameObject* m_pOwner = nullptr;
public:
	void SetOwner(GameObject* owner);
	GameObject* GetOwner() const { return m_pOwner; }
	virtual void Update();
};

