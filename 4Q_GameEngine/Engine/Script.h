#pragma once

using namespace ECS;

class Script
{
public:
	Script() = default;
	explicit Script(int parentEntityId)
		: m_ParentEntityId(parentEntityId)
	{}
	virtual ~Script() = default;

private:

protected:

	int m_ParentEntityId = 0;
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
	std::string m_ComponentName = "Script";
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Script, m_ParentEntityId)
};