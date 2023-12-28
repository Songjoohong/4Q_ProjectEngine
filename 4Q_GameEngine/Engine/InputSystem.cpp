#include "pch.h"
#include "InputSystem.h"

int VK_key[static_cast<int>(Key::KEY_END)] =
{
	VK_LBUTTON,
	VK_RBUTTON,
	VK_UP,
	VK_DOWN,
	'W',
	'A',
	'S',
	'D',
};
void InputSystem::Update(float deltaTime)
{
	HWND hWnd = GetFocus();
	if(hWnd != nullptr)
	{
		for (int i = 0; i < static_cast<int>(Key::KEY_END); i++)
		{
			isPressed[i] = GetAsyncKeyState(VK_key[i]) & 0x8000;
		}
	}
	

	for (int i = 0; i < static_cast<int>(Key::KEY_END); i++)
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

	for (int i = 0; i < static_cast<int>(Key::KEY_END); i++)
	{
		isPressed[i] = false;
	}
	for (int i = 0; i < static_cast<int>(Key::KEY_END); i++)
	{
		if (m_CurrentKeyState[i].KeyState == KeyState::STAY)
			std::cout << "KeyDown " << i << std::endl;
	}
}

KeyInfo& InputSystem::GetKeyState(const Key key)
{
	return m_CurrentKeyState[key];
}
