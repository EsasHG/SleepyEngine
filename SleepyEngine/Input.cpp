#include "Input.h"
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


void Input::HandleKeyEvents(int key)
{
	for (auto it = m_KeyCallbacks[key].begin(); it != m_KeyCallbacks[key].end(); ++it)
	{
		(*it)();
	}
}

void Input::HandleMousePosEvents(double xPos, double yPos)
{
	for (auto it = m_MousePosCallbacks.begin(); it != m_MousePosCallbacks.end(); ++it)
	{
		(*it)(xPos, yPos);
	}
}

void Input::HandleMouseButtonEvents(int button)
{
	for (auto it = m_MouseButtonCallbacks[button].begin(); it != m_MouseButtonCallbacks[button].end(); ++it)
	{
		(*it)();
	}
	
}

