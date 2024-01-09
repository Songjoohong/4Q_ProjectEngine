#pragma once
#include "ISingleton.h"

enum Key
{
	LBUTTON = 0,
	RBUTTON,
	UP,
	DOWN,
	W,
	A,
	S,
	D,

	KEY_END,

};

enum class KeyState
{
	NONE,
	ENTER,
	STAY,
	EXIT,
};

struct KeyInfo
{
	KeyState KeyState = KeyState::NONE;
	float Duration = 0.f;
};
class InputSystem : public ISingleton<InputSystem>
{
public:
	InputSystem() = default;
	virtual ~InputSystem() override = default;

public:
	InputSystem(const InputSystem&) = delete;
	void operator=(const InputSystem&) = delete;
	InputSystem(const InputSystem&&) = delete;
	void operator=(const InputSystem&&) = delete;

private:
	bool isPressed[Key::KEY_END] = {};
	KeyInfo m_CurrentKeyState[Key::KEY_END] = {};
	KeyState m_PreviousKeyState[Key::KEY_END] = { KeyState::NONE, };

public:
	void Update(float deltaTime);
	KeyInfo& GetKeyState(Key key);
};

