#include "UiLayer.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Entity.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "TransformSystem.h"

UiLayer::UiLayer()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO* io = &ImGui::GetIO(); (void)io;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		style.WindowBorderSize = 0.0f;
	}
}

void UiLayer::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiDockNodeFlags docking_Flags = 0;//ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), docking_Flags);
	ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
}

void UiLayer::Run(double deltaTime, Entity* sceneEntity)
{
	bool windowFocused = false;

	if (showRenderWindow)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Game Window", &showRenderWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavInputs);// | ImGuiWindowFlags_MenuBar);
		//if (ImGui::BeginMainMenuBar())
		//{
		//	ImGui::BeginMenu("Test");
		//	//ImGui::Button("Play", ImVec2(25, 25));
		//	ImGui::MenuItem("Test2", "CTRL+P");
		//	ImGui::EndMenu();
		//	ImGui::EndMainMenuBar();
		//}
		ImGui::BeginChild("GameRender",ImVec2(0,0), false, ImGuiWindowFlags_NoNavInputs);
		ImVec2 crSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)sceneTexture, crSize, ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsWindowFocused())
		{
			windowFocused = true;
			ImGui::SetNextFrameWantCaptureMouse(false);
			ImGui::SetNextFrameWantCaptureKeyboard(false);
		}
		contentRegionSize = glm::vec2(crSize.x, crSize.y);
		ImGui::EndChild();
		ImGui::End();
		ImGui::PopStyleVar();
	}

	if (showTestWindow3)
	{
		ImGui::Begin("Info", &showTestWindow3);
		ImGui::Text("Framerate: %.2f", 1 / deltaTime);
		ImGui::Text("Time Per Frame: %.3fms", deltaTime* 1000);

		if(ImGui::GetIO().WantCaptureKeyboard)
			ImGui::Text("WantCaptureKeyboard: True");
		else
			ImGui::Text("WantCaptureKeyboard: False");

		if(ImGui::GetIO().WantCaptureMouse)
			ImGui::Text("WantCaptureMouse: True");
		else
			ImGui::Text("WantCaptureMouse: False");

		if(windowFocused)
			ImGui::Text("Window focused: True");
		else
			ImGui::Text("Window focused: False");

		ImGui::End();
	}

	

	//object tree window
	if (showObjectTreeWindow)
	{
		entityToSelect = nullptr;

		id = 0;

		ImGui::Begin("Object tree", &showObjectTreeWindow);
		//ImGui::PushID(++id);
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGuiTreeNodeFlags node_flags = base_flags;
		node_flags |= ImGuiTreeNodeFlags_DefaultOpen;


		TransformComponent* transform = sceneEntity->GetComponent<TransformComponent>();

		//UI changes if there are no children
		if (transform->children.empty())
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;


		//check selection
		auto it = std::find(selectedEntities.begin(), selectedEntities.end(), sceneEntity);
		const bool is_selected = (it != selectedEntities.end());
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		//make node
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)transform, node_flags, sceneEntity->m_Name.c_str());

		//add to selection next frame
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			entityToSelect = sceneEntity;
		
		//handle children
		if (node_open)
		{
			if (!transform->children.empty())
			{
				for (TransformComponent* t : transform->children)
				{
					ProcessTreeNode(base_flags, t->m_Entity);
				}
			}
			ImGui::TreePop();
		}

		//ImGui::PopID();

		ImGui::End();
	}

	if (showObjectWindow) //Needs to be after Object tree window for parent button to work
	{
		ImGui::Begin("Object Details", &showObjectWindow);
		if (selectedEntities.size() == 0)
		{
			ImGui::Text("No objects selected!");

		}
		else if (selectedEntities.size() == 1)
		{
			Entity* entity = selectedEntities[0];

			ImGui::Text(entity->m_Name.c_str());

			//transform component
			TransformComponent* transform = entity->GetComponent<TransformComponent>();

			glm::vec3 pos = TransformSystem::GetPosition(transform);
			glm::vec3 rot = TransformSystem::GetRotation(transform);
			glm::vec3 scale = TransformSystem::GetScale(transform);

			ImGui::InputFloat3("Position", (float*)&pos);
			ImGui::InputFloat3("Rotation", (float*)&rot);
			ImGui::InputFloat3("Scale", (float*)&scale);

			TransformSystem::SetPosition(transform->m_Entity, pos);
			TransformSystem::SetRotation(transform->m_Entity, rot);
			TransformSystem::SetScale(transform->m_Entity, scale);

			if (transform->parent)
			{
				ImGui::Text("Parent:");
				if (ImGui::Button(transform->parent->m_Entity->m_Name.c_str()))
				{
					entityToSelect = transform->parent->m_Entity;
				}
			}
		}
		else
		{
			ImGui::Text("Multiple objects selected!");
		}

		if (sceneSelected)
		{
			ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
			ImGui::ColorEdit3("Quad Color", (float*)&quadColor);
		}
		if (pointLightSelected)
		{
			ImGui::SliderFloat3("Point Light Position", (float*)&pointLightPos, -10.0f, 10.0f);
			ImGui::ColorEdit3("Point Light Diffuse", (float*)&pointLightDiffuse);
		}
		if (dirLightSelected)
		{
			ImGui::InputFloat3("Dir Light Direction", (float*)&dirLightDir);
			ImGui::ColorEdit3("Dir Light Diffuse", (float*)&dirLightDiffuse);
		}
		ImGui::End();
	}



	//update selection for next frame
	if (entityToSelect != nullptr)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		if (ImGui::GetIO().KeyCtrl)
			selectedEntities.push_back(entityToSelect);		// CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
		{
			selectedEntities.clear();						// Click to single-select
			selectedEntities.push_back(entityToSelect);
		}
	}

}

void UiLayer::ProcessTreeNode(const ImGuiTreeNodeFlags& base_flags, Entity* entity)
{

	//ImGui::PushID(entity->m_Name.c_str());

	ImGuiTreeNodeFlags node_flags = base_flags;

	TransformComponent* transform = entity->GetComponent<TransformComponent>();

	//UI changes if there are no children
	if (transform->children.empty())
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet; // | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	
	//check selection
	auto it = std::find(selectedEntities.begin(), selectedEntities.end(), entity);
	const bool is_selected = (it != selectedEntities.end());
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	
	//make node
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)transform, node_flags, entity->m_Name.c_str());
	//add to selection next frame
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		entityToSelect = entity;
	

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
		{

		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("ENTITYPOINTER", &entity, sizeof(Entity));
		//ImGui::Text(entity->m_Name.c_str());
		ImGui::EndDragDropSource();
	}


	//handle children
	if (node_open)
	{
		if (!transform->children.empty())
		{
			for (TransformComponent* t : transform->children)
			{
				ProcessTreeNode(base_flags, t->m_Entity);
			}
		}
		ImGui::TreePop();
	}

	//ImGui::PopID();
}

void UiLayer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentContext);
	}

}
