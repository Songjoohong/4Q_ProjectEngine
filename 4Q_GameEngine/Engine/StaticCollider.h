#pragma once
#include "Collider.h"

class StaticCollider
	:public Collider
{
public:
	StaticCollider(BoxCollider* owner);
	~StaticCollider() = default;

public:
	virtual void Initialize() override;
	virtual void SetFilterData() override;

public:
	PxRigidStatic* m_Rigid;
};

