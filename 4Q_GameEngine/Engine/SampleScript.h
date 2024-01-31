#pragma once
#include "ECS.h"
#include "Script.h"
#include "Transform.h"

class SampleScript : public Script
{
public:
	SampleScript() = default;
	SampleScript(int entityId, int parentEntityId = 0) : Script(entityId, parentEntityId) {}

	virtual void Update() override
	{
		//Entity* owner = GetOwner(); // �� ��ũ��Ʈ�� ��ƼƼ�� ����Ŵ
		//World* world = GetOwner()->getWorld(); // ��ƼƼ�� ����
		//ComponentHandle< Transform> transformComponent = owner->get<Transform>(); // ��ƼƼ�� ������Ʈ�� ����
	}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SampleScript, m_EntityId, m_ParentEntityId)
};
