#pragma once
#include "RigidInstance.h"

using namespace physx;

class Model;
class StaticRigidInstance
	:public RigidInstance
{
public: 
	StaticRigidInstance()=default;
	~StaticRigidInstance() = default;

public:

public:
	PxBoxGeometry	 m_Scale;
	PxTransform		 m_Transform;
	PxRigidStatic*	 m_Rigid;
};

