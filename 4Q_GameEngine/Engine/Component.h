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

protected:
	int m_Code = 0;

public:
	void SetOwner(GameObject* owner);
	void SetCode(GameObject* owner) const ;
	GameObject* GetOwner() const { return m_pOwner; }
};

