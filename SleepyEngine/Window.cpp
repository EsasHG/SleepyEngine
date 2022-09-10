#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "InputManager.h"


Window::Window(int width, int height, const char* title) : m_Width(width), m_Height(height)
{
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Sleepy Engine", NULL, NULL);

	if (!m_Window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	const char* description;
	int code = glfwGetError(&description);
	if (code != GLFW_NO_ERROR)
	{
		std::cout << description << std::endl;
		glfwTerminate();
		return;
	}

	InputManager::GetInstance().Init(m_Window);
	InputManager::GetInstance().AddWindowResizeCallback(std::bind(&Window::WindowResizeCallback, this, std::placeholders::_1, std::placeholders::_2));

}

void Window::WindowResizeCallback(int width, int height)
{
	m_Width = width;
	m_Height = height;
	glViewport(0, 0, width, height);
}


bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}

