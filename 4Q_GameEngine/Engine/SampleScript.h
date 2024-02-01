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
		//Entity* owner = GetOwner(); // 이 스크립트의 엔티티를 가리킴
		//World* world = GetOwner()->getWorld(); // 엔티티의 월드
		//ComponentHandle< Transform> transformComponent = owner->get<Transform>(); // 엔티티의 컴포넌트에 접근
	}
	std::string m_ComponentName = "SampleScript";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SampleScript, m_ParentEntityId)
};
