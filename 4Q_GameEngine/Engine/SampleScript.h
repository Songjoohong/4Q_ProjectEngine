#pragma once
#include "ECS.h"
#include "Script.h"
#include "Transform.h"

class SampleScript : public Script
{
public:
	SampleScript() = default;
	SampleScript(int parentEntityId) : Script(parentEntityId) {}

	virtual void Update() override
	{
		//Entity* owner = GetOwner(); // �� ��ũ��Ʈ�� ��ƼƼ�� ����Ŵ
		//World* world = GetOwner()->getWorld(); // ��ƼƼ�� ����
		//ComponentHandle< Transform> transformComponent = owner->get<Transform>(); // ��ƼƼ�� ������Ʈ�� ����
	}
	std::string m_ComponentName = "SampleScript";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SampleScript, m_ParentEntityId)
};
