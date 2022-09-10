#pragma once
#include <vector>
#include "Input.h"
struct GLFWwindow;

static InputManager* s_InputManager;

class InputManager
{

public:
	static InputManager& GetInstance() {
		if(!s_InputManager)

		s_InputManager = new InputManager();
		return *s_InputManager;
	}

	void RunEvents();

	void Init(GLFWwindow* window);
	void AddWindowResizeCallback(std::function<void(int, int)> func);

private:
	InputManager() {};
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void FramebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);


	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	void CursorPosCallbackImpl(GLFWwindow* window, double xPos, double yPos);

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods);

	std::vector<Input*> m_Inputs;
	std::vector< std::function<void(int, int)>> m_FramebufferSizeCallbacks;
	bool bInitialized = false;

	void AddInputComponent(Input* i);
	friend class Input;
};

