#include "Input.h"
#include <iostream>
#include "InputManager.h"

Input::Input(GLFWwindow* window) : m_Window(window)
{
	InputManager::GetInstance().AddInputComponent(this);

}

void Input::AddKeyBinding(int key, std::function<void()> func)
{
	m_KeyCallbacks[key].push_back(func);
}

void Input::AddMousePosBinding(std::function<void(double, double)> func)
{
	m_MousePosCallbacks.push_back(func);
}

void Input::AddMouseButtonBinding(int button, std::function<void()> func )
{
	m_MouseButtonCallbacks[button].push_back(func);
}

void Input::RunEvents()
{
	for (int key : activeKeys)
	{
		for (auto it = m_KeyCallbacks[key].begin(); it != m_KeyCallbacks[key].end(); ++it)
		{
			(*it)();
		}
	}

	for (int button : activeMouseButtons)
	{
		for (auto it = m_MouseButtonCallbacks[button].begin(); it != m_MouseButtonCallbacks[button].end(); ++it)
		{
			(*it)();
		}
	}
}


void Input::HandleKeyEvents(int key, int action)
{
	if (action == GLFW_PRESS)
	{
		activeKeys.push_back(key);
	}
	else if (action == GLFW_RELEASE)
	{
		remove(activeKeys.begin(), activeKeys.end(), key);

		auto it = std::find(activeKeys.begin(), activeKeys.end(), key);
		if (it != activeKeys.end())
			activeKeys.erase(it);
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
		activeMouseButtons.push_back(button);
	}
	if (action == GLFW_RELEASE)
	{
		auto it = std::find(activeMouseButtons.begin(), activeMouseButtons.end(), button);
		if (it != activeMouseButtons.end())
			activeMouseButtons.erase(it);
	}
}

