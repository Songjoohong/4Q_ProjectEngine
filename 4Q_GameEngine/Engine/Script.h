#pragma once

using namespace ECS;

class Script
{
public:
	explicit Script(ECS::Entity* owner)
		: m_pOwner(owner)
	{}
	virtual ~Script() = default;


	Entity* m_pOwner = nullptr;

public:
	ECS::Entity* GetOwner() const { return m_pOwner; }

	virtual void Update(float deltaTime) {}
	virtual void Awake() {}
	virtual void LateUpdate() {}
	virtual void FixedUpdate() {}
	virtual void Start() {}
	virtual void OnDestroyed() {}
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}
	virtual void OnCollisionStay() {}
};