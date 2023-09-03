#include "InputManager.h"
#include <GLFW/glfw3.h>
#include "ImGui/imgui.h"

namespace Sleepy
{
	void InputManager::Init(GLFWwindow* window)
	{
		if (!bInitialized)
		{
			glfwSetKeyCallback(window, KeyCallback);
			glfwSetCursorPosCallback(window, CursorPosCallback);
			glfwSetMouseButtonCallback(window, MouseButtonCallback);
			glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
			bInitialized = true;
		}
	}
	void InputManager::RunEvents()
	{
#ifdef _SHOWUI
		bDispatchKeyboardEvents = !ImGui::GetIO().WantCaptureKeyboard;
		bDispatchMouseEvents = !ImGui::GetIO().WantCaptureMouse;
#else
		bDispatchKeyboardEvents = true;
		bDispatchMouseEvents = true;

#endif // _SHOWUI

		if (bDispatchKeyboardEvents)
		{
			for (InputComponent* i : m_Inputs)
			{
				i->RunKeyEvents();
			}
		}

		if (bDispatchMouseEvents)
		{
			for (InputComponent* i : m_Inputs)
			{
				i->RunKeyEvents();
			}
		}
	}

	void InputManager::RunEvents(InputComponent& input)
	{
	#ifdef _SHOWUI
		bDispatchKeyboardEvents = !ImGui::GetIO().WantCaptureKeyboard;
		bDispatchMouseEvents = !ImGui::GetIO().WantCaptureMouse;
	#else
		bDispatchKeyboardEvents = true;
		bDispatchMouseEvents = true;
	
	#endif // _SHOWUI
	
		if (bDispatchKeyboardEvents)
		{
			input.RunKeyEvents();
		}
	
		if (bDispatchMouseEvents)
		{
			input.RunKeyEvents();
		}
	}
	
	void InputManager::AddInputComponent(InputComponent* i)
	{
		m_Inputs.push_back(i);
	}
	
	void InputManager::AddWindowResizeCallback(std::function<void(int, int)> func)
	{
		m_FramebufferSizeCallbacks.push_back(func);
	}

	void InputManager::RemoveInputComponent(InputComponent* input)
	{
		auto it = std::find(m_Inputs.begin(), m_Inputs.end(), input);
		if (it != m_Inputs.end())
			m_Inputs.erase(it);
	}
	
	
	void InputManager::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		GetInstance().FramebufferSizeCallbackImpl(window, width, height);
	}
	
	void InputManager::FramebufferSizeCallbackImpl(GLFWwindow* window, int width, int height)
	{
		for (auto it = m_FramebufferSizeCallbacks.begin(); it != m_FramebufferSizeCallbacks.end(); ++it)
		{
			(*it)(width, height);
		}
	}
	
	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GetInstance().KeyCallbackImpl(window, key, scancode, action, mods);
	}
	
	void InputManager::KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (bDispatchKeyboardEvents)
		{
			for (InputComponent* i : m_Inputs)
			{
				i->HandleKeyEvents(key, action);
			}
		}
	}
	
	
	void InputManager::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		GetInstance().CursorPosCallbackImpl(window, xPos, yPos);
	
	}
	
	void InputManager::CursorPosCallbackImpl(GLFWwindow* window, double xPos, double yPos)
	{
		if (bDispatchMouseEvents)
		{
			for (InputComponent* i : m_Inputs)
			{
				i->HandleMousePosEvents(xPos, yPos);
			}
		}
	}
	
	
	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		GetInstance().MouseButtonCallbackImpl(window, button, action, mods);
	}
	
	void InputManager::MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods)
	{
		if (bDispatchMouseEvents)
		{
			for (InputComponent* i : m_Inputs)
			{
				i->HandleMouseButtonEvents(button, action);
			}
		}
	}
}