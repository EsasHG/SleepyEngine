#include "Application.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

int Application::Run()
{
	int windowWidth = 1000, windowHeight = 1000;
	
	
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Sleepy Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	const char* description;
	int code = glfwGetError(&description);
	if (code != GLFW_NO_ERROR)
	{
		std::cout << description << std::endl;
		glfwTerminate();
	}

	if (!gladLoadGL())
	{
		std::cout << "Glad did not load successfully!" << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.7f, 0.3f, 0.6f,1.0f);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
