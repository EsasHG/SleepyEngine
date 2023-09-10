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
#include "InputManager.h"
#include "Components/CameraComponent.h"
#include "EditorCamera.h"
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
		EditorCamera* camera = &m_scenes[0]->CreateGameObject<EditorCamera>("Camera");
		camera->bActive = true;
		//camera->AddComponent<UpdateComponent>();
		//Camera* playerCamera = &m_scenes[0]->CreateGameObject<Camera>("Player Camera");
		//playerCamera->AddComponent<UpdateComponent>();

		renderer->SetCamera(camera->m_Camera);

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
			// 
			//If play or stop button pressed
			if (ui->Run(deltaTime, m_scenes[0]))
			{
				//if we are playing and should stop
				if (s_Playing)
				{
					//Delete objects that should only exist while playing
					for (Scene* scene : m_scenes)
					{
						scene->EndPlay();

					}
					//Change back to editor camera
					renderer->SetCamera(camera->m_Camera);

					//Reset so it's ready for next time play button is pressed.
					firstGameFrame = true;


					camera->bActive = true;
				}
				s_Playing = !s_Playing;
			}
			
			renderer->BeginFrame(ui->contentRegionSize);
	#else
			renderer->BeginFrame(glm::vec2(window->GetWidth(),window->GetHeight()));

	#endif // _SHOWUI;

			glfwPollEvents();

			InputManager::GetInstance().RunEvents();
			
			if (s_Playing && firstGameFrame)
			{
				for (Scene* scene : m_scenes)
				{
					scene->BeginPlay();
					auto camView = scene->m_registry.view<CameraComponent>();
					for (auto [entity, camera] : camView.each())
					{
						if (camera.bPossessOnStart)
							renderer->SetCamera(&camera);
					}
				}
				firstGameFrame = false;
				//renderer->SetCamera(playerCamera->m_Camera);
				camera->bActive = false;
			}

			if(s_Playing)
			{
				for (Scene* scene : m_scenes)
					scene->Update(deltaTime);
			}
			else
			{
				camera->Update(deltaTime);
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
			for (Scene* scene : m_scenes)
				scene->Draw();
			renderer->DrawCubemap();
			renderer->DrawSceneTextureOnScreen(renderer->EndFrame());
			s_Playing = true;
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