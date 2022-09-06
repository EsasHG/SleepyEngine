#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <vector>
class Input
{
public:
	Input(GLFWwindow* window);
	void AddKeyBinding(int key, std::function<void()> func);
	void AddMousePosBinding(std::function<void(double, double)> func);
	void AddMouseButtonBinding(int button, std::function<void()> func);
private:
	void RunEvents();

	void HandleKeyEvents(int key, int action);
	void HandleMousePosEvents(double xPos, double yPos);
	void HandleMouseButtonEvents(int button, int action);

	GLFWwindow* m_Window;
	std::vector<int> activeKeys;
	std::vector<int> activeMouseButtons;


	std::map<int, std::vector<std::function<void()>>> m_KeyCallbacks;
	std::vector<std::function<void(double, double)>> m_MousePosCallbacks;
	std::map<int, std::vector<std::function<void()>>> m_MouseButtonCallbacks;

	friend class InputManager;
};

