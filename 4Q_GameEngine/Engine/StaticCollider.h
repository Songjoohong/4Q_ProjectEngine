#pragma once
#include "Collider.h"

class StaticCollider
	:public Collider
{
public:
	StaticCollider(BoxCollider* owner);
	virtual ~StaticCollider() override;

public:
	virtual void Initialize() override;
	virtual void UpdatePosition()override;

public:
	PxRigidStatic* m_Rigid;
};

