#pragma once

class OnCollision
{
public:
	virtual ~OnCollision();

	virtual void OnCollisionEnter() = 0;
	virtual void OnCollisionStay() = 0;
	virtual void OnCollisionExit() = 0;
};