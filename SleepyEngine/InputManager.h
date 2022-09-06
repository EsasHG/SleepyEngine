#pragma once
#include <vector>
#include "Input.h"
class GLFWwindow;

static InputManager* s_InputManager;

class InputManager
{

public:
	static InputManager& GetInstance() {
		if(!s_InputManager)

		s_InputManager = new InputManager();
		return *s_InputManager;
	}
	bool Init(GLFWwindow* window);

	void RunEvents();
private:
	InputManager() {};

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	void CursorPosCallbackImpl(GLFWwindow* window, double xPos, double yPos);

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods);


	GLFWwindow* m_Window;

	std::vector<Input*> inputs;
	bool bInitialized = false;

	void AddInputComponent(Input* i);
	friend class Input;
};

