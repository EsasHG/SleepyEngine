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
#include <stdio.h>
#include <assimp/Importer.hpp>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Renderer.h"
#include "Input.h"
#include "InputManager.h"
#include "Camera.h"
#include "Window.h"
#include "UiLayer.h"
#include "Model.h"
//TEMP

Application::~Application()
{
	delete window;
	delete renderer;
}

double Application::BeginFrame()
{
	double time = glfwGetTime();
	double deltaTime = time - prevFrameTime;
	prevFrameTime = time;
	renderer->BeginFrame();

	return deltaTime;
}

int Application::Run() 
{
	window = new Window(800, 600, "Sleepy Engine");
	renderer = new Renderer(glm::vec2(window->GetWidth(), window->GetHeight()));
	window->EnableImGui();
	
	Camera* camera = new Camera();
	renderer->SetCamera(camera);


	while (!window->ShouldClose())
	{
		double deltaTime = BeginFrame();
		
		glfwPollEvents();

		InputManager::GetInstance().RunEvents();
		camera->Run(deltaTime);
		renderer->Draw(deltaTime);

		EndFrame();
	}
	return 0;
}

void Application::EndFrame()
{
	renderer->EndFrame();
	window->SwapBuffers();
}
