#pragma once
#include "ECS.h"
class PlayerInputSystem :
    public ECS::EntitySystem
{
public:
    virtual ~PlayerInputSystem() override = default;

    virtual void Tick(ECS::World* world, ECS::DefaultTickData data) override;

};

