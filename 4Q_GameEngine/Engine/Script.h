#pragma once

using namespace ECS;

class Script
{
public:
	Script() = default;

	virtual ~Script() = default;

public:
	std::string m_ComponentName = "Script";

  virtual void Update(float deltaTime) {}
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