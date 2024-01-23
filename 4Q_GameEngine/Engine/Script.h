#pragma once

using namespace ECS;

class Script
{
public:
	Script(ECS::Entity* owner)
		: m_pOwner(owner)
	{}
	virtual ~Script() = default;

private:
	Entity* m_pOwner = nullptr;

public:
	ECS::Entity* GetOwner() const { return m_pOwner; }

	virtual void Update() {}
	virtual void Awake() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}
	virtual void Start() {}
	virtual void OnDestroyed() {}
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}
};