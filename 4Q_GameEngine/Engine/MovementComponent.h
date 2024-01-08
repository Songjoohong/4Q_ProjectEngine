#pragma once
#include "Component.h"
class MovementComponent :
    public Component
{
public:
    MovementComponent();
	~MovementComponent() override = default;

private:
    int m_Key[4] = { 'W', 'A', 'S', 'D' };
    float m_Speed = 0.f;
public:
    
};

