#include "InputManager.h"
#include <GLFW/glfw3.h>
#include "Input.h"

bool InputManager::Init(GLFWwindow* window)
{
	if (!bInitialized)
	{
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		bInitialized = true;
	}

	return bInitialized;
}

void InputManager::AddInputComponent(Input* i)
{
	inputs.push_back(i);
}


void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GetInstance().KeyCallbackImpl(window, key, scancode, action, mods);
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	GetInstance().CursorPosCallbackImpl(window, xPos, yPos);

}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GetInstance().MouseButtonCallbackImpl(window, button, action, mods);
}

void InputManager::CursorPosCallbackImpl(GLFWwindow* window, double xPos, double yPos)
{
	for (Input* i : inputs)
	{
		i->HandleMousePosEvents(xPos, yPos);
	}
}


void InputManager::KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	for (Input* i : inputs)
	{
		i->HandleKeyEvents(key);
	}
}

void InputManager::MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods)
{
	for (Input* i : inputs)
	{
		i->HandleMouseButtonEvents(button);
	}
}

