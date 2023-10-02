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
#include "Components/MeshComponent.h"
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
			UiInfo info = ui->Run(deltaTime, m_scenes[0]);
			if (info.bTogglePlay)
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
					//renderer->SetCamera(camera->m_Camera);

					//Reset so it's ready for next time play button is pressed.
					firstGameFrame = true;

					camera->bActive = true;
				}
				s_Playing = !s_Playing;
			}
			if (info.bNewWindow)
			{
				Scene* s = new Scene();
				m_scenes.push_back(s);
				Entity& ent = s->CreateGameObject<Entity>("Camera");
				ent.AddComponent<CameraComponent>();
				srand(glfwGetTime());
				ent.SetRotation(glm::vec3(rand() % 180, rand() % 180, rand() % 180));
				Entity& planet = s->CreateGameObject<Entity>("planet");
				planet.AddComponent<MeshComponent>("planet_0", "Mars", "default");
				planet.SetPosition(glm::vec3(0, 0, -10.0f));
				planet.SetRotation(glm::vec3(rand() % 180, rand() % 180, rand() % 180));
				//renderer->AddFramebuffer();

			}
#endif // _SHOWUI;

			glfwPollEvents();

			InputManager::GetInstance().RunEvents();
			
			if (s_Playing && firstGameFrame)
			{
				for (Scene* scene : m_scenes)
				{
					scene->BeginPlay();
					//auto camView = scene->m_registry.view<CameraComponent>();
					//for (auto [entity, camera] : camView.each())
					//{
					//	if (camera.bPossessOnStart)
					//		renderer->SetCamera(&camera);
					//}
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

			renderer->BeginFrame();

	#ifdef _SHOWUI


			unsigned int sceneID = 0;
			for (int i = 0; i < info.renderWindows.size(); i++)
			{
				RenderWindow& window = info.renderWindows[i];
				if (window.bOpen)
				{

					/*for (Scene* s : m_scenes)
					{*/
					auto camView = m_scenes[sceneID]->m_registry.view<CameraComponent>();

					//if there is no active camera, we want to use an inactive one instead. 
					// If there is no camera, we create one and use that

					bool camSet = false;
					CameraComponent* emergencyCam = nullptr;
					for (auto [entity, camera] : camView.each())
					{
						emergencyCam = &camera;
						if (camera.m_Entity->bActive)
						{
							camera.m_BufferData.bufferHeigth = window.contentRegionSize.x;
							camera.m_BufferData.bufferWidth = window.contentRegionSize.y;
							renderer->PrepDraw(camera);
							//renderer->SetCamera(&camera);
							camSet = true;
							break;
						}
					}
					if (!camSet)
					{
						if (emergencyCam)
						{
							emergencyCam->m_BufferData.bufferHeigth = window.contentRegionSize.x;
							emergencyCam->m_BufferData.bufferWidth = window.contentRegionSize.y;
							renderer->PrepDraw(*emergencyCam);
							//renderer->SetCamera(emergencyCam);
						}
						else
						{
							Entity& ent = m_scenes[sceneID]->CreateGameObject<Entity>("auto camera");
							CameraComponent& newCam = ent.AddComponent<CameraComponent>();
							newCam.m_BufferData.bufferHeigth = window.contentRegionSize.x;
							newCam.m_BufferData.bufferWidth = window.contentRegionSize.y;
							renderer->PrepDraw(newCam);
						}
					}

					m_scenes[sceneID]->Draw();
					renderer->DrawCubemap();

					window.sceneTexture = renderer->EndBufferRender();

					sceneID++;
				}
				else
				{
					Scene* removedScene = m_scenes[sceneID];
					m_scenes.erase(m_scenes.begin() + sceneID);
				
					delete removedScene;

					//TODO: remove framebuffer as well?
				}
			}
			ui->EndFrame();
	#else
			renderer->BeginFrame(glm::vec2(window->GetWidth(), window->GetHeight()));
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