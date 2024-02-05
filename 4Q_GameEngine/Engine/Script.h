#pragma once

using namespace ECS;

class Script
{
public:
	Script() = default;

	virtual ~Script() = default;

private:

protected:

public:
	//ECS::Entity* GetOwner() const { return m_pOwner; }

	std::string m_ComponentName = "Script";
	virtual void Update() {}
	virtual void Awake() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}
	virtual void Start() {}
	virtual void OnDestroyed() {}
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Script, m_ComponentName)
};