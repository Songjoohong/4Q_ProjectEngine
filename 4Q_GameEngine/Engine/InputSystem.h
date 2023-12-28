#pragma once

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
	KeyState KeyState;
	float Duration = 0.f;
};
class InputSystem
{
public:
	InputSystem() = default;
	~InputSystem() = default;

	InputSystem(const InputSystem&) = delete;
	void operator=(const InputSystem&) = delete;
	InputSystem(const InputSystem&&) = delete;
	void operator=(const InputSystem&&) = delete;

private:
	bool isPressed[Key::KEY_END] = {};
	KeyInfo m_CurrentKeyState[Key::KEY_END] = {};
	KeyState m_PreviousKeyState[Key::KEY_END] = {};

public:
	void Update(float deltaTime);
	KeyInfo& GetKeyState(Key key);
};

