#pragma once
#include "ECS.h"
#include "Script.h"
#include "Transform.h"

class SampleScript : public Script
{
public:
	SampleScript(Entity* ent) : Script(ent)
	{}

	virtual void Update() override
	{
		Entity* owner = GetOwner(); // �� ��ũ��Ʈ�� ��ƼƼ�� ����Ŵ
		World* world = GetOwner()->getWorld(); // ��ƼƼ�� ����
		ComponentHandle<Component::Transform> transformComponent = owner->get<Component::Transform>(); // ��ƼƼ�� ������Ʈ�� ����
	}
};