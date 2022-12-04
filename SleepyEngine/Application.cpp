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
#include <entt/entt.hpp>
#include <functional>

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
#include "Scene.h"
#include "UiLayer.h"


Application::~Application()
{
	delete window;
#ifdef _SHOWUI
	delete ui;
#endif // _SHOWUI
	delete renderer;
}

double Application::BeginFrame()
{
	double time = glfwGetTime();
	double deltaTime = time - prevFrameTime;
	prevFrameTime = time;

	return deltaTime;
}

int Application::Run() 
{
	window = new Window(1920, 1080, "Sleepy Engine");
#ifdef _SHOWUI

	ui = new UiLayer();
	window->EnableImGui();
#endif // _SHOWUI
	InputManager::GetInstance().AddWindowResizeCallback(std::bind(&Application::FramebufferResizeCallback, this, std::placeholders::_1, std::placeholders::_2));

	renderer = new Renderer(glm::vec2(window->GetWidth(), window->GetHeight()));
	
	Camera* camera = new Camera();
	renderer->SetCamera(camera);
	Scene scene;
	while (!window->ShouldClose())
	{
		scene.CleanupRegistry();


		double deltaTime = BeginFrame();
#ifdef _SHOWUI;

		ui->BeginFrame();
		ui->Run(deltaTime, scene.m_SceneEntity);
		renderer->BeginFrame(ui->contentRegionSize);
#else
		renderer->BeginFrame(glm::vec2(window->GetWidth(),window->GetHeight()));

#endif // _SHOWUI;

		glfwPollEvents();

		InputManager::GetInstance().RunEvents();
		camera->Run(deltaTime);
		scene.Update();

#ifdef _SHOWUI;
		if (ui->RenderWindowOpen())
		{
			renderer->PrepDraw(deltaTime);
			scene.Draw();
			ui->sceneTexture = renderer->EndFrame();
		}
		ui->EndFrame();
#else
		renderer->PrepDraw(deltaTime);
		scene.Draw();
		renderer->DrawSceneTextureOnScreen(renderer->EndFrame());
#endif // _SHOWUI;

		EndFrame();
	}
	return 0;
}

void Application::EndFrame()
{

	window->SwapBuffers();
}


void Application::FramebufferResizeCallback(int x, int y)
{
	//renderer->ResizeViewport(ui->contentRegionSize.x, ui->contentRegionSize.y);
}
