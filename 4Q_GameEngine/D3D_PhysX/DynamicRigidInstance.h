#pragma once

using namespace physx;

class Model;
class DynamicRigidInstance
{
public:
	DynamicRigidInstance()=default;
	~DynamicRigidInstance()=default;

public:
	void Update();

public:
	Model*	m_pOwner;
	PxBoxGeometry	 m_Scale;
	PxTransform		 m_Transform;
	PxRigidDynamic*  m_Rigid;
};	

