#include "Application.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Renderer.h"
#include "Input.h"
#include "InputManager.h"


//TEMP
glm::vec3 position = glm::vec3(0.0f,0.0f,4.0f);
glm::vec3 front = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 right;
glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);


void TestFunc2()
{
	std::cout << "TESTFUNC2" << std::endl;

}

int Application::Run()
{

	int windowWidth = 800, windowHeight = 600;

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

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	InputManager::GetInstance().Init(window);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int shaderId = Renderer::CreateShader("Shaders/SingleColor.vert", "Shaders/SingleColor.frag");

	glUseProgram(shaderId);



	glm::mat4 projection = glm::perspective(0.5f, (float)windowWidth / windowHeight, 0.1f, 10.0f);
	Renderer::SetShaderUniformMat4(shaderId, "projection", projection);

	Input i(window);
	i.AddKeyBinding(GLFW_KEY_A, std::bind(&Application::TestFunc, this));
	i.AddMousePosBinding(std::bind(&Application::TestFunc3, this, std::placeholders::_1,std::placeholders::_2));
	i.AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, std::bind(&Application::MouseButtonPressed, this));

	while (!glfwWindowShouldClose(window))
	{
		
		glClearColor(0.7f, 0.3f, 0.6f,1.0f);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glUseProgram(shaderId);
		
		glm::mat4 view = glm::lookAt(position, position + front, up);
		Renderer::SetShaderUniformMat4(shaderId, "view", view);


		glm::vec3 quadColor = glm::vec3(glm::sin(glfwGetTime() - 1) / 2 + 0.5, glm::sin(glfwGetTime()) / 2 + 0.5, glm::sin(glfwGetTime() + 1) / 2 + 0.5);
		Renderer::SetShaderUniformVec3(shaderId, "color", quadColor);

		glm::mat4 model = glm::mat4(1.0f);
		Renderer::SetShaderUniformMat4(shaderId, "model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
}

void Application::TestFunc()
{
	std::cout << "TESTFUNC" << std::endl;
}

void Application::TestFunc3(double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	if (mousePressed)
	{
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		std::cout << "x offset " << xoffset << "y offset " << yoffset << std::endl;
		yaw += xoffset;
		pitch += yoffset;
	
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);
	
		right = glm::normalize(glm::cross(front, up));
	
	}
	//std::cout << "xPos: " << xPos << "yPos: " << yPos << std::endl;
}

void Application::MouseButtonPressed()
{
	std::cout << "Mouse Pressed!" << std::endl;
	mousePressed = true;
}
