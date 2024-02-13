 #pragma once
#include "ISingleton.h"

#define InputM InputManager::GetInstance()

enum Key
{
	LBUTTON = 0,
	MBUTTON,
	RBUTTON,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	W,
	A,
	S,
	D,
	Q,
	E,
	CTRL,
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
class InputManager final : public ISingleton<InputManager>
{
public:
	InputManager() = default;
	virtual ~InputManager() override = default;

public:
	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;
	InputManager(const InputManager&&) = delete;
	void operator=(const InputManager&&) = delete;

private:
	bool m_IsPressed[Key::KEY_END] = {false, };
	bool m_IsCursorCameraMode = false;
	KeyInfo m_CurrentKeyState[Key::KEY_END] = {};
	KeyState m_PreviousKeyState[Key::KEY_END] = { KeyState::NONE, };
	POINT m_CurrentCursorPos = { 0,0 };
	POINT m_PreviousCursorPos = { 0,0 };
	UINT m_Width = 0;
	UINT m_Height = 0;
public:
	void Initialize(UINT width, UINT height);
	void Update(float deltaTime);
	void SetCameraMode(bool isCameraMode) { m_IsCursorCameraMode = isCameraMode; }
	[[nodiscard]] KeyInfo& GetKeyState(Key key);
	[[nodiscard]] bool GetKey(Key key) const;
	[[nodiscard]] bool GetKeyDown(Key key) const;
	[[nodiscard]] bool GetKeyUp(Key key) const;
	[[nodiscard]] bool GetMouseButton(int key) const;
	[[nodiscard]] bool GetMouseButtonDown(int key) const;
	[[nodiscard]] bool GetMouseButtonUp(int key) const;
	[[nodiscard]] POINT GetMouseMove() const;
	[[nodiscard]] POINT GetMousePos() const;
};

