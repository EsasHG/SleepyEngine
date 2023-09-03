#include "InputComponent.h"
#include <iostream>
#include "InputManager.h"
#include "Entity.h"

namespace Sleepy
{
	InputComponent::InputComponent(Entity* entity)
	{
		InputManager::GetInstance().AddInputComponent(this);
		m_Entity = entity;
		m_componentType = INPUT;
	}

	InputComponent::~InputComponent()
	{
		InputManager::GetInstance().RemoveInputComponent(this);
	}
	
	void InputComponent::AddKeyBinding(int key, SLE_INPUT_MODE inputMode, std::function<void()> func)
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
	
	void InputComponent::AddMousePosBinding(std::function<void(double, double)> func)
	{
		m_MousePosCallbacks.push_back(func);
	}

	void InputComponent::AddMouseScrollBinding(std::function<void(double, double)> func)
	{
		m_MouseScrollCallbacks.push_back(func);
	}
	
	void InputComponent::AddMouseButtonBinding(int button, SLE_INPUT_MODE inputMode, std::function<void()> func )
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
	
	void InputComponent::RunKeyEvents()
	{
		if (!m_Entity->bActive) return;

		for (int key : m_ActiveKeys)
		{
			for (auto it = m_HeldKeyCallbacks[key].begin(); it != m_HeldKeyCallbacks[key].end(); ++it)
			{
				if (*it != nullptr)
					(*it)();
			}
		}
	}
	
	
	void InputComponent::RunMouseEvents()
	{
		if (!m_Entity->bActive) return;

		for (int button : m_ActiveMouseButtons)
		{
			for (auto it = m_HeldMouseButtonCallbacks[button].begin(); it != m_HeldMouseButtonCallbacks[button].end(); ++it)
			{
				if(*it != nullptr)
					(*it)();
			}
		}
	}
	
	void InputComponent::HandleKeyEvents(int key, int action)
	{
		if (!m_Entity->bActive) return;

		if (action == GLFW_PRESS)
		{
			m_ActiveKeys.push_back(key);
			for (auto it = m_PressedKeyCallbacks[key].begin(); it != m_PressedKeyCallbacks[key].end(); ++it)
			{
				if (*it != nullptr)
					(*it)();
			}
		}
		else if (action == GLFW_RELEASE)
		{
			m_ActiveKeys.erase(std::remove(m_ActiveKeys.begin(), m_ActiveKeys.end(), key), m_ActiveKeys.end());
	
	
			for (auto it = m_ReleasedKeyCallbacks[key].begin(); it != m_ReleasedKeyCallbacks[key].end(); ++it)
			{
				if (*it != nullptr)
					(*it)();
			}
		}
	}
	
	void InputComponent::HandleMousePosEvents(double xPos, double yPos)
	{
		if (!m_Entity->bActive) return;

		for (auto it = m_MousePosCallbacks.begin(); it != m_MousePosCallbacks.end(); ++it)
		{
			if (*it != nullptr)
				(*it)(xPos, yPos);
		}
	}

	void InputComponent::HandleMouseScrollEvents(double xOffset, double yOffset)
	{
		if (!m_Entity->bActive) return;

		for (auto it = m_MousePosCallbacks.begin(); it != m_MousePosCallbacks.end(); ++it)
		{
			if (*it != nullptr)
				(*it)(xOffset, yOffset);
		}
	}
	
	void InputComponent::HandleMouseButtonEvents(int button, int action)
	{
		if (!m_Entity->bActive) return;

		if (action == GLFW_PRESS)
		{
			for (auto it = m_PressedMouseButtonCallbacks[button].begin(); it != m_PressedMouseButtonCallbacks[button].end(); ++it)
			{
				if (*it != nullptr)
					(*it)();
			}
			m_ActiveMouseButtons.push_back(button);
		}
		if (action == GLFW_RELEASE)
		{
			for (auto it = m_ReleasedMouseButtonCallbacks[button].begin(); it != m_ReleasedMouseButtonCallbacks[button].end(); ++it)
			{
				if (*it != nullptr)
					(*it)();
			}
			m_ActiveMouseButtons.erase(std::remove(m_ActiveMouseButtons.begin(), m_ActiveMouseButtons.end(), button), m_ActiveMouseButtons.end());
		}
	}
}