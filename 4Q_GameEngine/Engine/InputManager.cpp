#include "pch.h"
#include "InputManager.h"

int VK_key[static_cast<int>(Key::KEY_END)] =
{
	VK_LBUTTON,
	VK_MBUTTON,
	VK_RBUTTON,
	VK_UP,
	VK_DOWN,
	'W',
	'A',
	'S',
	'D',
};
void InputManager::Update(float deltaTime)
{
	HWND hWnd = GetFocus();
	if(hWnd != nullptr)
	{
		for (int i = 0; i < Key::KEY_END; i++)
		{
			isPressed[i] = GetAsyncKeyState(VK_key[i]) & 0x8000;
		}
		m_PreviousCursorPos = m_CurrentCursorPos;
		GetCursorPos(&m_CurrentCursorPos);
		if (m_CurrentCursorPos.x == 0 || m_CurrentCursorPos.x >= 2560)
		{
			SetCursorPos(1280, m_CurrentCursorPos.y);
			m_PreviousCursorPos = { 1280, m_CurrentCursorPos.y };
		}

		if(m_CurrentCursorPos.y == 0 || m_CurrentCursorPos.y >= 1439)
		{
			SetCursorPos(m_CurrentCursorPos.x, 720);
			m_PreviousCursorPos = { m_CurrentCursorPos.x, 720 };
		}
			
	}
	

	for (int i = 0; i < Key::KEY_END; i++)
	{
		if(isPressed[i])
		{
			if(m_PreviousKeyState[i] == KeyState::ENTER)
			{
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i].KeyState = KeyState::STAY;
				m_CurrentKeyState[i].Duration += deltaTime;
			}
			else if(m_PreviousKeyState[i] == KeyState::STAY)
			{ 
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i].Duration += deltaTime;
			}
			else
			{
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i].KeyState = KeyState::ENTER;
			}
		}
		else
		{
			if(m_PreviousKeyState[i] == KeyState::EXIT)
			{
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i].KeyState = KeyState::NONE;
			}
			else if(m_PreviousKeyState[i] == KeyState::ENTER)
			{
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i].KeyState = KeyState::EXIT;
			}
			else if(m_PreviousKeyState[i] == KeyState::STAY)
			{
				m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
				m_CurrentKeyState[i] = { KeyState::EXIT, 0 };
			}
		}
	}

	for (int i = 0; i < Key::KEY_END; i++)
	{
		isPressed[i] = false;
	}

	for (int i = 0; i < Key::KEY_END; i++)
	{
		if (m_CurrentKeyState[i].KeyState == KeyState::STAY)
			std::cout << "KeyDown " << i << std::endl;
	}
}

KeyInfo& InputManager::GetKeyState(const Key key)
{
	return m_CurrentKeyState[key];
}

bool InputManager::GetKey(Key key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::ENTER
		|| m_CurrentKeyState[key].KeyState == KeyState::STAY
		|| m_CurrentKeyState[key].KeyState == KeyState::EXIT;
}

bool InputManager::GetKeyDown(Key key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::ENTER;
}

bool InputManager::GetKeyUp(Key key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::EXIT;
}

bool InputManager::GetMouseButton(int key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::ENTER
		|| m_CurrentKeyState[key].KeyState == KeyState::STAY
		|| m_CurrentKeyState[key].KeyState == KeyState::EXIT;
}

bool InputManager::GetMouseButtonDown(int key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::ENTER;
}

bool InputManager::GetMouseButtonUp(int key) const
{
	return m_CurrentKeyState[key].KeyState == KeyState::EXIT;
}

POINT InputManager::GetMouseMove() const
{
	//스크린 좌표계임을 고려해 설정해 주었음.
	return { m_CurrentCursorPos.x - m_PreviousCursorPos.x, m_PreviousCursorPos.y - m_CurrentCursorPos.y };
}

POINT InputManager::GetMousePos() const
{
	return m_CurrentCursorPos;
}
