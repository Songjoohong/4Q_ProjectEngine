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

	void SetFiltercData();

public:
	PxRigidStatic* m_Rigid;
};

