#include "UiLayer.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Entity.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Components/LightComponent.h"
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
		SetupObjectTree(sceneEntity);
	

	if (showObjectWindow) //Needs to be after Object tree window for parent button to work
	{
		SetupObjectWindow();
		
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

	//update parenting
	if (newIndex != -1 && newParentEntity && entityToMove)
		ReorderObjectTree(newParentEntity, newIndex, entityToMove);
}

void UiLayer::SetupObjectTree(Entity* sceneEntity)
{

	//reset reparenting variables
	entityToSelect = nullptr;
	newParentEntity = nullptr;
	entityToMove = nullptr;
	newIndex = -1;

	id = 0;

	ImGui::Begin("Object tree", &showObjectTreeWindow);
	//ImGui::PushID(++id);
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	ImGuiTreeNodeFlags node_flags = base_flags;
	node_flags |= ImGuiTreeNodeFlags_DefaultOpen;

	TransformComponent* transform = sceneEntity->GetComponent<TransformComponent>();

	//UI changes if there are no children
	if (transform->m_children.empty())
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;


	//check selection
	auto it = std::find(selectedEntities.begin(), selectedEntities.end(), sceneEntity);
	const bool is_selected = (it != selectedEntities.end());
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
	//ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0.4f, 0.5f, 0.95f, 0.900f));

	//make node
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)transform, node_flags, sceneEntity->m_Name.c_str());

	//add to selection next frame
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		entityToSelect = sceneEntity;

	//Drag target on Scene
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
		{
			Entity* ent = (Entity*)payload->Data;
			if (ent)
			{
				entityToMove = ent;
				newParentEntity = sceneEntity;
				newIndex = 0;
			}
		}
		ImGui::EndDragDropTarget();
	}

	//handle children
	if (node_open)
	{
		if (!transform->m_children.empty())
		{
			//drag target above first child
			ImGui::PushID(++id);
			ImGui::InvisibleButton("dragTarget", ImVec2(ImGui::GetContentRegionAvail().x, 5));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
				{
					Entity* ent = (Entity*)payload->Data;
					if (ent)
					{
						entityToMove = ent;
						newParentEntity = sceneEntity;
						newIndex = 0;
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			for (TransformComponent* t : transform->m_children)
				ProcessTreeNode(base_flags, t->m_Entity);
		}
		ImGui::TreePop();
	}
	//ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();


}

void UiLayer::ProcessTreeNode(const ImGuiTreeNodeFlags& base_flags, Entity* entity)
{
	ImGuiTreeNodeFlags node_flags = base_flags;

	TransformComponent* transform = entity->GetComponent<TransformComponent>();

	//UI changes if there are no children
	if (transform->m_children.empty())
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
	
	//drag target on tree
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
		{
			Entity* ent = (Entity*)payload->Data;
			if (ent)
			{
				entityToMove = ent;
				newParentEntity = entity;
				newIndex = 0;
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("ENTITYPOINTER", entity, sizeof(Entity));
		ImGui::Text(entity->m_Name.c_str());
		ImGui::EndDragDropSource();
	}

	//handle children
	if (node_open)
	{
		if (!transform->m_children.empty())
		{
			//drag target above first child
			ImGui::PushID(++id);
			ImGui::InvisibleButton("dragTarget", ImVec2(ImGui::GetContentRegionAvail().x, 5));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
				{
					Entity* ent = (Entity*)payload->Data;

					//Currently don't use the payload. Don't like that :(
					if (ent)
					{
						entityToMove = ent;
						newParentEntity = entity;
						newIndex = 0;
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			for (TransformComponent* t : transform->m_children)
				ProcessTreeNode(base_flags, t->m_Entity);	
		}
		ImGui::TreePop();
	}

	//drag target below
	ImGui::PushID(++id);
	ImGui::InvisibleButton("dragTarget",ImVec2(ImGui::GetContentRegionAvail().x,5));
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
		{

			Entity* ent = (Entity*)payload->Data;

			//Currently don't use the payload. Don't like that :(
			if (ent)
			{
				entityToMove = ent;
				TransformComponent* targetTransform = entity->GetComponent<TransformComponent>();
				newParentEntity = targetTransform->m_parent->m_Entity;

				std::vector<TransformComponent*>& childGroup = targetTransform->m_parent->m_children;

				auto it = std::find(childGroup.begin(), childGroup.end(), entity->GetComponent<TransformComponent>());

				if (it != childGroup.end())
					newIndex = std::distance(childGroup.begin(), it) + 1;
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();
}

void UiLayer::ReorderObjectTree(Entity* newParent, int newIndex, Entity* entityToMove)
{
	if (entityToMove == newParent) { return; } //trying to drag right below itself

	TransformComponent* parentTransform = newParent->GetComponent<TransformComponent>();
	TransformSystem::SetParent(parentTransform->m_Entity, entityToMove);

	std::vector<TransformComponent*>& childGroup = parentTransform->m_children;

	int oldIndex = childGroup.size() - 1;

	if (oldIndex > newIndex)
		std::rotate(childGroup.rend() - oldIndex - 1, childGroup.rend() - oldIndex, childGroup.rend() - newIndex);
}

void UiLayer::SetupObjectWindow()
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
		ImGui::Spacing();
		ImGui::Separator();

		//static char strName[128];
		//ImGui::InputText("Object Name", strName, IM_ARRAYSIZE(strName),0,0,(void*)entity->m_Name.c_str());
		//entity->m_Name = strName;

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		if (transform)
		{
			ShowTransformComp(transform);
		}
		DirLightComponent* dirLight = entity->GetComponent<DirLightComponent>();
		if (dirLight)
		{
			ShowDirLightComp(dirLight);
		}
		PointLightComponent* pointLight = entity->GetComponent<PointLightComponent>();
		if (pointLight)
		{
			ShowPointLightComp(pointLight);
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
	ImGui::End();
}

void UiLayer::ShowTransformComp(TransformComponent* transform)
{
	//transform component
	ImGui::Spacing();
	ImGui::Text("Transform Component");
	ImGui::Spacing();
	glm::vec3 pos = TransformSystem::GetPosition(transform);
	glm::vec3 rot = TransformSystem::GetRotation(transform);
	glm::vec3 scale = TransformSystem::GetScale(transform);

	ImGui::DragFloat3("Position", (float*)&pos, 0.01f, -100, 100);
	ImGui::DragFloat3("Rotation", (float*)&rot, 0.01f, -180, 180);
	ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, -100, 100);

	TransformSystem::SetPosition(transform->m_Entity, pos);
	TransformSystem::SetRotation(transform->m_Entity, rot);
	TransformSystem::SetScale(transform->m_Entity, scale);

	if (transform->m_parent )
	{
		ImGui::Text("Parent:");
		if (ImGui::Button(transform->m_parent->m_Entity->m_Name.c_str()))
		{
			entityToSelect = transform->m_parent->m_Entity;
		}
		ImGui::SameLine();
		if (ImGui::Button("Unparent"))
		{
			TransformSystem::Unparent(transform->m_Entity);
		}
	}
	ImGui::Spacing();
	ImGui::Separator();

}

void UiLayer::ShowDirLightComp(DirLightComponent* dirLight)
{
	ImGui::Spacing();
	ImGui::Text("Directional Light Component");
	ImGui::Spacing();
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	ImGui::DragFloat3("Direction", (float*)&dirLight->m_direction, 0.01f,-1.0f, 1.0f);
	ImGui::ColorEdit3("Ambient", (float*)&dirLight->m_ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&dirLight->m_diffuse);
	ImGui::ColorEdit3("Specular", (float*)&dirLight->m_specular);
}

void UiLayer::ShowPointLightComp(PointLightComponent* pointLight)
{
	ImGui::Spacing();
	ImGui::Text("Point Light Component");
	ImGui::Spacing();
	ImGui::ColorEdit3("Ambient", (float*)&pointLight->m_ambient);
	ImGui::ColorEdit3("Diffuse", (float*)&pointLight->m_diffuse);
	ImGui::ColorEdit3("Specular", (float*)&pointLight->m_specular);
	ImGui::DragFloat("Constant", &pointLight->m_constant, 0.005f, 0.0f, 2.0f);
	ImGui::DragFloat("Linear", &pointLight->m_linear, 0.005f, 0.0f,2.0f);
	ImGui::DragFloat("Quadratic", &pointLight->m_quadratic, 0.005f,0.0f,2.0f);

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
