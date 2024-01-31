#pragma once

using namespace ECS;

class Script
{
public:
	Script() = default;
	explicit Script(int entityId, int parentEntityId = 0)
		: m_EntityId(entityId), m_ParentEntityId(parentEntityId)
	{}
	virtual ~Script() = default;

private:

protected:

	int m_ParentEntityId = 0;
	int m_EntityId = 0;
public:
	//ECS::Entity* GetOwner() const { return m_pOwner; }

	virtual void Update() {}
	virtual void Awake() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}
	virtual void Start() {}
	virtual void OnDestroyed() {}
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Script, m_EntityId, m_ParentEntityId)
};