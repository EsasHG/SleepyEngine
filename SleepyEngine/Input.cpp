#include "Input.h"
#include <iostream>
#include "InputManager.h"

namespace Sleepy
{
	Input::Input()
	{
		InputManager::GetInstance().AddInputComponent(this);
	}
	
	void Input::AddKeyBinding(int key, SLE_INPUT_MODE inputMode, std::function<void()> func)
	{
		switch (inputMode)
		{
		case(SLE_HELD):
			m_HeldKeyCallbacks[key].push_back(func);
			break;
		case(SLE_PRESSED):
			m_PressedKeyCallbacks[key].push_back(func);
			break;
	
		case(SLE_RELEASED):
			m_ReleasedKeyCallbacks[key].push_back(func);
			break;
		default:
			std::cout << "Invalid input mode";
		};
	}
	
	void Input::AddMousePosBinding(std::function<void(double, double)> func)
	{
		m_MousePosCallbacks.push_back(func);
	}
	
	void Input::AddMouseButtonBinding(int button, SLE_INPUT_MODE inputMode, std::function<void()> func )
	{
		switch (inputMode)
		{
		case(SLE_HELD):
			m_HeldMouseButtonCallbacks[button].push_back(func);
			break;
		case(SLE_PRESSED):
			m_PressedMouseButtonCallbacks[button].push_back(func);
			break;
	
		case(SLE_RELEASED):
			m_ReleasedMouseButtonCallbacks[button].push_back(func);
			break;
		default:
			std::cout << "Invalid input mode";
		};
	}
	
	void Input::RunKeyEvents()
	{
		for (int key : m_ActiveKeys)
		{
			for (auto it = m_HeldKeyCallbacks[key].begin(); it != m_HeldKeyCallbacks[key].end(); ++it)
			{
				(*it)();
			}
		}
	}
	
	
	void Input::RunMouseEvents()
	{
		for (int button : m_ActiveMouseButtons)
		{
			for (auto it = m_HeldMouseButtonCallbacks[button].begin(); it != m_HeldMouseButtonCallbacks[button].end(); ++it)
			{
				(*it)();
			}
		}
	
	}
	
	void Input::HandleKeyEvents(int key, int action)
	{
		if (action == GLFW_PRESS)
		{
			m_ActiveKeys.push_back(key);
			for (auto it = m_PressedKeyCallbacks[key].begin(); it != m_PressedKeyCallbacks[key].end(); ++it)
			{
				(*it)();
			}
		}
		else if (action == GLFW_RELEASE)
		{
			m_ActiveKeys.erase(std::remove(m_ActiveKeys.begin(), m_ActiveKeys.end(), key), m_ActiveKeys.end());
	
	
			for (auto it = m_ReleasedKeyCallbacks[key].begin(); it != m_ReleasedKeyCallbacks[key].end(); ++it)
			{
				(*it)();
			}
		}
	}
	
	void Input::HandleMousePosEvents(double xPos, double yPos)
	{
		for (auto it = m_MousePosCallbacks.begin(); it != m_MousePosCallbacks.end(); ++it)
		{
			(*it)(xPos, yPos);
		}
	}
	
	void Input::HandleMouseButtonEvents(int button, int action)
	{
		if (action == GLFW_PRESS)
		{
			for (auto it = m_PressedMouseButtonCallbacks[button].begin(); it != m_PressedMouseButtonCallbacks[button].end(); ++it)
			{
				(*it)();
			}
			m_ActiveMouseButtons.push_back(button);
		}
		if (action == GLFW_RELEASE)
		{
			for (auto it = m_ReleasedMouseButtonCallbacks[button].begin(); it != m_ReleasedMouseButtonCallbacks[button].end(); ++it)
			{
				(*it)();
			}
			m_ActiveMouseButtons.erase(std::remove(m_ActiveMouseButtons.begin(), m_ActiveMouseButtons.end(), button), m_ActiveMouseButtons.end());
		}
	}
}