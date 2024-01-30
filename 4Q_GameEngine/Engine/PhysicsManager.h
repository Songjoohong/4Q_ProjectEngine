#pragma once
#include "ISingleton.h"

class StaticModel;
class Physics;
class PhysicsManager : public ISingleton<PhysicsManager>
{
public: 
	PhysicsManager() = default;
	~PhysicsManager() override = default;

public:
	void Initialize();
	void Update();

public:
	Physics* m_Physics;
};

