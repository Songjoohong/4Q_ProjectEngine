#pragma once
class IOnTrigger
{
public:
	virtual ~IOnTrigger();

	virtual void OnTriggerEnter() = 0;
	virtual void OnTriggerExit() = 0;
	virtual void OnTriggerStay() = 0;
};