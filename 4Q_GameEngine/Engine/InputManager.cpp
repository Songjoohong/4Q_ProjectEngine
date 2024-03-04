#include "pch.h"
#include "InputManager.h"

int VK_key[static_cast<int>(Key::KEY_END)] =
{
	VK_LBUTTON,
	VK_MBUTTON,
	VK_RBUTTON,
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,
	'W',
	'A',
	'S',
	'D',
	'Q',
	'E',
	VK_CONTROL,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
	VK_TAB
};

void InputManager::Initialize(UINT width, UINT height)
{
	m_Height = height;
	m_Width = width;
}

void InputManager::Update(float deltaTime)
{
	HWND hWnd = GetFocus();
	if(hWnd != nullptr)
	{
		for (int i = 0; i < Key::KEY_END; i++)
		{
			bool isDisalbed = false;
			if (!m_DisabledKeys.empty())
			{
				for (const auto& disabledKey : m_DisabledKeys)
				{
					if (i == disabledKey)
					{
						isDisalbed = true;
						break;
					}
				}
			}

			if(!isDisalbed)
			{
				m_IsPressed[i] = GetAsyncKeyState(VK_key[i]) & 0x8000;
			}
		}

		m_PreviousCursorPos = m_CurrentCursorPos;
		GetCursorPos(&m_CurrentCursorPos);
		ScreenToClient(hWnd, &m_CurrentCursorPos);

		if(m_IsCursorCameraMode)
		{
			if (m_CurrentCursorPos.x <= 0 || m_CurrentCursorPos.x >= m_Width - 100)
			{
				POINT clientPoint = { static_cast<long>(m_Width) / 2, m_CurrentCursorPos.y };
				m_CurrentCursorPos = clientPoint;
				m_PreviousCursorPos = m_CurrentCursorPos;
				ClientToScreen(hWnd, &clientPoint);
				SetCursorPos(clientPoint.x, clientPoint.y);
			}

			if (m_CurrentCursorPos.y <= 0 || m_CurrentCursorPos.y >= m_Height - 100)
			{
				POINT clientPoint = { m_CurrentCursorPos.x, static_cast<long>(m_Height) / 2 };
				m_CurrentCursorPos = clientPoint;
				m_PreviousCursorPos = m_CurrentCursorPos;
				ClientToScreen(hWnd, &clientPoint);
				SetCursorPos(clientPoint.x, clientPoint.y);
			}

			
		}
	}

	for (int i = 0; i < Key::KEY_END; i++)
	{
		m_PreviousKeyState[i] = m_CurrentKeyState[i].KeyState;
		if(m_IsPressed[i])
		{
			if(m_PreviousKeyState[i] == KeyState::ENTER)
			{
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
				m_CurrentKeyState[i].KeyState = KeyState::ENTER;
			}
		}
		else
		{
			if(m_PreviousKeyState[i] == KeyState::EXIT)
			{
				m_CurrentKeyState[i].KeyState = KeyState::NONE;
			}
			else if(m_PreviousKeyState[i] == KeyState::ENTER)
			{
				m_CurrentKeyState[i].KeyState = KeyState::EXIT;
			}
			else if(m_PreviousKeyState[i] == KeyState::STAY)
			{
				m_CurrentKeyState[i] = { KeyState::EXIT, 0 };
			}
		}
	}

	for (bool& pressed : m_IsPressed)
	{
		pressed = false;
	}
}

void InputManager::UnlockKey()
{
	m_DisabledKeys.clear();
}

void InputManager::LockKey(Key key)
{
	m_DisabledKeys.push_back(key);
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
