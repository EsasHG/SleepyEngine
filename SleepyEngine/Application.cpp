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

#include "Renderer.h"
#include "Input.h"
#include "InputManager.h"
#include "Camera.h"

//TEMP


int Application::Run()
{
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

	shaderId = Renderer::CreateShader("Shaders/SingleColor.vert", "Shaders/SingleColor.frag");

	glUseProgram(shaderId);



	glm::mat4 projection = glm::perspective(0.5f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
	Renderer::SetShaderUniformMat4(shaderId, "projection", projection);
	Camera camera;
	Input i;
	
	InputManager::GetInstance().AddeWindowResizeCallback(std::bind(&Application::WindowResizeCallback, this, std::placeholders::_1, std::placeholders::_2));
	
	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		double deltaTime = time - prevFrameTime;
		prevFrameTime = time;
		camera.Run(deltaTime);

		glClearColor(0.7f, 0.3f, 0.6f,1.0f);
		glViewport(0, 0, windowWidth, windowHeight);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glUseProgram(shaderId);
		glm::mat4 view = camera.GetViewMatrix();
		Renderer::SetShaderUniformMat4(shaderId, "view", view);


		glm::vec3 quadColor = glm::vec3(glm::sin(glfwGetTime() - 1) / 2 + 0.5, glm::sin(glfwGetTime()) / 2 + 0.5, glm::sin(glfwGetTime() + 1) / 2 + 0.5);
		Renderer::SetShaderUniformVec3(shaderId, "color", quadColor);

		glm::mat4 model = glm::mat4(1.0f);
		Renderer::SetShaderUniformMat4(shaderId, "model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
		InputManager::GetInstance().RunEvents();
	}
}

void Application::WindowResizeCallback(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);

	glm::mat4 projection = glm::perspective(0.5f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
	Renderer::SetShaderUniformMat4(shaderId, "projection", projection);
}
