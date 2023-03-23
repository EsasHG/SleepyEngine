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

namespace Sleepy
{
	bool Application::s_Playing = false;
	Application::Application()
	{
		window = new Window(1920, 1080, "Sleepy Engine");
#ifdef _SHOWUI
		ui = new UiLayer();
		window->EnableImGui();
#else
		s_Playing = true;

#endif // _SHOWUI
		InputManager::GetInstance().AddWindowResizeCallback(std::bind(&Application::FramebufferResizeCallback, this, std::placeholders::_1, std::placeholders::_2));

		renderer = new Renderer(glm::vec2(window->GetWidth(), window->GetHeight()));
		prevFrameTime = glfwGetTime();
	}

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
		Camera* camera = new Camera();
		renderer->SetCamera(camera);

		//Scene scene;
		//SetScene(new Scene());
		while (!window->ShouldClose())
		{
			for (Scene* scene : m_scenes)
				scene->CleanupRegistry();

			double deltaTime = BeginFrame();
	#ifdef _SHOWUI

			ui->BeginFrame();
			//ui->Run(deltaTime, nullptr);
			for (Scene* scene : m_scenes)
			{
				if (ui->Run(deltaTime, scene))
				{
					if (s_Playing)
					{
						for (Scene* scene : m_scenes)
						{
							for(Entity* e : scene->gameEntities)
								scene->MarkForDeletion(*e);
						}
						firstGameFrame = true;
					}
					s_Playing = !s_Playing;
				}
			}
			renderer->BeginFrame(ui->contentRegionSize);
	#else
			renderer->BeginFrame(glm::vec2(window->GetWidth(),window->GetHeight()));

	#endif // _SHOWUI;

			glfwPollEvents();

			InputManager::GetInstance().RunEvents();
			camera->Run(deltaTime);
			
			if (s_Playing)
			{
				if (firstGameFrame)
				{
					for (Scene* scene : m_scenes)
						scene->BeginPlay();
					firstGameFrame = false;
				}

				for (Scene* scene : m_scenes)
					scene->Update(deltaTime);
			}

	#ifdef _SHOWUI
			if (ui->RenderWindowOpen())
			{
				renderer->PrepDraw(deltaTime);
				for (Scene* scene : m_scenes)
					scene->Draw();
				
				renderer->DrawCubemap();
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

}