#pragma once
#include "Component.h"
#include "Vector3D.h"

class TransformComponent :
    public Component
{
public:
    TransformComponent();
    ~TransformComponent() override = default;

private:
    Vector3D m_WorldTransform = { 0.f,0.f,0.f };
    Vector3D m_RelativeTransform = { 0.f,0.f,0.f };

    std::string m_Name = "TransformComponent";
public:
    Vector3D& GetPosition() { return m_RelativeTransform; }
    void SetPosition(const Vector3D& pos) { m_RelativeTransform = pos; }
    void MovePosition(const Vector3D& pos) { m_RelativeTransform += pos; }
    //void Update();
};

