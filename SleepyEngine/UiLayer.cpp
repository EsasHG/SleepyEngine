#include "UiLayer.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "Window.h"
#include "Entity.h"
#include "Scene.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "TransformSystem.h"
#include "ModelLibrary.h"
#include "Fonts/IconsFontAwesome6.h"

#define PROJECTDIR
namespace Sleepy
{

	UiLayer::UiLayer()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			style.WindowBorderSize = 0.0f;
		}
		std::string font = "SleepyEngine\\Fonts\\fa-regular-400.ttf";
		font = _SOLUTIONDIR + font;
		io.Fonts->AddFontDefault();

		ImFontConfig config;
		config.MergeMode = true;
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		io.Fonts->AddFontFromFileTTF(font.c_str(), 13.0f, &config, icon_ranges);
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

	bool UiLayer::Run(double deltaTime, Scene* scene)
	{
		Entity* sceneEntity = scene->m_SceneEntity;
		bool windowFocused = false;
		bool bTogglePlay = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows"))
			{
				//ImGui::Button("Play", ImVec2(25, 25));
				if (ImGui::MenuItem("Main", "", &showRenderWindow)) {}
				if (ImGui::MenuItem("Object Tree", "", &showObjectTreeWindow)) {}
				if (ImGui::MenuItem("Object Details", "", &showObjectWindow)) {}
				if (ImGui::MenuItem("Performance", "", &showPerformanceWindow)) {}
				if (ImGui::MenuItem("Engine Settings", "", &engineSettingsOpen)) {}
				if (ImGui::MenuItem("Style Editor", "", &styleEditorOpen)) {}
				if (ImGui::MenuItem("Metrics", "", &metricsWindowOpen)) {}
				if (ImGui::MenuItem("Debug Log ", "", &debugLogWindowOpen)) {}
				if (ImGui::MenuItem("Stack Tool", "", &stackToolWindowOpen)) {}
				if (ImGui::MenuItem("ImGui User Guide", "", &userGuideOpen)) {}

				ImGui::EndMenu();
			}
			if (!playing)
			{
				if (ImGui::Button("PLAY",ImVec2(50,20)))
				{
					bTogglePlay = true;
					playing = true;
				}
			}
			else
			{
				if (ImGui::Button("STOP", ImVec2(50, 20)))
				{
					bTogglePlay = true;
					playing = false;
				}
			}

			ImGui::EndMainMenuBar();
		}

		if (engineSettingsOpen)
		{
			ImGui::Begin("Engine Settings", &engineSettingsOpen);
			ImGui::ShowStyleSelector("Style Selector");
			ImGui::ShowFontSelector("Font Selector");
			ImGui::End();
		}
		if (styleEditorOpen)
		{
			ImGui::Begin("Style Editor", &styleEditorOpen);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}
		if (metricsWindowOpen)
			ImGui::ShowMetricsWindow(&metricsWindowOpen);
		if (debugLogWindowOpen)
			ImGui::ShowDebugLogWindow(&debugLogWindowOpen);
		if (stackToolWindowOpen)
			ImGui::ShowStackToolWindow(&stackToolWindowOpen);
		if (userGuideOpen)
		{
			ImGui::Begin("User Guide", &userGuideOpen);
			ImGui::ShowUserGuide();
			ImGui::End();
		}
		if (showRenderWindow)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			renderWindowOpen = ImGui::Begin("Game Window", &showRenderWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavInputs);// | ImGuiWindowFlags_MenuBar);

			ImGui::BeginChild("GameRender", ImVec2(0, 0), false, ImGuiWindowFlags_NoNavInputs);
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

		if (showPerformanceWindow)
		{
			ImGui::Begin("Info", &showPerformanceWindow);
			ImGui::Text("Framerate: %.2f", 1 / deltaTime);
			ImGui::Text("Time Per Frame: %.3fms", deltaTime * 1000);
			//frametimes.push(deltaTime);
			////if (frametimes.size() > 100)
			//	//frametimes.pop();
			//char overlay[32];
			//sprintf_s(overlay, "Framerate: %.2f", 1 / deltaTime);
			//ImGui::PlotLines("Frames", &frametimes.front(), frametimes.size(),0,overlay,0.0f,1.0f);
			if (ImGui::GetIO().WantCaptureKeyboard)
				ImGui::Text("WantCaptureKeyboard: True");
			else
				ImGui::Text("WantCaptureKeyboard: False");

			if (ImGui::GetIO().WantCaptureMouse)
				ImGui::Text("WantCaptureMouse: True");
			else
				ImGui::Text("WantCaptureMouse: False");

			if (windowFocused)
				ImGui::Text("Window focused: True");
			else
				ImGui::Text("Window focused: False");

			ImGui::End();
		}

		//object tree window
		if (showObjectTreeWindow && sceneEntity)
			SetupObjectTree(*sceneEntity);

		if (showObjectWindow) //Needs to be after Object tree window for parent button to work
			SetupObjectWindow(scene);

		if (showAssetsWindow)
			SetupAssetsWindow();

		//update selection for next frame
		if (entityToSelect != nullptr)
		{

			// Update selection state
			// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
			if (!ImGui::GetIO().KeyCtrl)						// CTRL+click to toggle
			{
				selectedEntities.clear();
				sceneSelected = false;
			}

			if (sceneEntity && entityToSelect == sceneEntity)
				sceneSelected = true;

			selectedEntities.push_back(entityToSelect->GetHandle());
		}

		//update parenting
		if (newIndex != -1 && newParentEntity && entityToMove)
			ReorderObjectTree(*newParentEntity, newIndex, *entityToMove);


		return bTogglePlay;
	}

	void UiLayer::SetupObjectTree(Entity& sceneEntity)
	{

		//reset reparenting variables
		entityToSelect = nullptr;
		newParentEntity = nullptr;
		entityToMove = nullptr;
		newIndex = -1;
		oldIndex = -1;

		id = 0;

		ImGui::Begin("Object tree", &showObjectTreeWindow);
		//ImGui::PushID(++id);
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGuiTreeNodeFlags node_flags = base_flags;
		node_flags |= ImGuiTreeNodeFlags_DefaultOpen;

		//UI changes if there are no children
		if (!sceneEntity.HasChildren())
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;

		//check selection
		auto it = std::find(selectedEntities.begin(), selectedEntities.end(), sceneEntity.GetHandle());
		const bool is_selected = (it != selectedEntities.end());
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
		//ImGui::PushStyleColor(ImGuiCol_DragDropTarget, ImVec4(0.4f, 0.5f, 0.95f, 0.900f));

		//make node
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)&sceneEntity, node_flags, sceneEntity.m_Name.c_str());

		//add to selection next frame
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			entityToSelect = &sceneEntity;

		//Drag target on Scene
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
			{
				Entity* ent = (Entity*)payload->Data;
				if (ent)
				{
					entityToMove = ent;
					newParentEntity = &sceneEntity;
					newIndex = 0;
				}
			}
			ImGui::EndDragDropTarget();
		}

		//handle children
		if (node_open)
		{
			if (sceneEntity.HasChildren())
			{
				//drag target above first child
				ImGui::PushID(++id);
				ImGui::InvisibleButton("dragTarget", ImVec2(ImGui::GetContentRegionAvail().x, 5)); //TODO: Find dynamic value instead of 5
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
					{
						Entity* ent = (Entity*)payload->Data;
						if (ent)
						{
							entityToMove = ent;
							newParentEntity = &sceneEntity;
							newIndex = 0;
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();

				for (auto child : sceneEntity.GetChildren())
					ProcessTreeNode(base_flags, *child);
			}
			ImGui::TreePop();
		}
		ImGui::PopStyleVar();
		ImGui::End();
	}

	void UiLayer::ProcessTreeNode(const ImGuiTreeNodeFlags& base_flags, Entity& entity)
	{
		ImGuiTreeNodeFlags node_flags = base_flags;

		//UI changes if there are no children
		if (!entity.HasChildren())
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet; // | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//check selection
		auto it = std::find(selectedEntities.begin(), selectedEntities.end(), entity.GetHandle());
		const bool is_selected = (it != selectedEntities.end());
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		//make node
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)&entity, node_flags, entity.m_Name.c_str());
		//add to selection next frame
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			entityToSelect = &entity;

		//drag target on tree
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYPOINTER"))
			{
				Entity* ent = (Entity*)payload->Data;
				if (ent)
				{
					entityToMove = ent;
					newParentEntity = &entity;
					newIndex = 0;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("ENTITYPOINTER", &entity, sizeof(Entity));
			ImGui::Text(entity.m_Name.c_str());
			ImGui::EndDragDropSource();
		}

		//handle children
		if (node_open)
		{
			if (entity.HasChildren())
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
							newParentEntity = &entity;
							newIndex = 0;
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();

				for (auto child : entity.GetChildren())
					ProcessTreeNode(base_flags, *child);
			}
			ImGui::TreePop();
		}

		//drag target below
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
					newParentEntity = &entity.GetParent();

					int i = 0;
					for (Entity* child : entity.GetParent().GetChildren())
					{

						i++;
						if (*child == *ent)
							oldIndex = i;
						if (*child == entity) // will this work?
						{
							newIndex = i;
							break;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
	}

	void UiLayer::ReorderObjectTree(Entity& newParent, int newIndex, Entity& entityToMove)
	{
		if (entityToMove == newParent) { return; } //trying to drag right below itself

		entityToMove.SetParent(newParent);

		if (oldIndex != -1 && newIndex >= oldIndex)
			newIndex--;
		newParent.MoveChild(entityToMove, newIndex);
	}

	void UiLayer::SetupObjectWindow(Scene* scene)
	{
		ImGui::Begin("Object Details", &showObjectWindow);
		if (selectedEntities.size() == 0)
		{
			ImGui::Spacing();
			ImGui::Text("No objects selected!");
		}
		else if (selectedEntities.size() == 1)
		{
			
			Entity* entity = scene->GetEntity(selectedEntities[0]);
			if (!entity)
			{
				selectedEntities.pop_back();
				ImGui::End();
				return;
			}
			ImGui::Spacing();
			ImGui::Text(entity->m_Name.c_str());
			if (!sceneSelected)
			{
				if (ImGui::Button("Delete"))
				{
					SceneBase::MarkForDeletion(*entity);
					selectedEntities.pop_back();
					ImGui::End();
					return;
				}
			}
			if (ImGui::BeginCombo("##label","Add Component"))
			{
				std::vector<std::string> v;
					
				for (std::string comp : validComponents)
				{
					bool selected = false;
					if (comp == "TransformComponent" && !entity->Has<TransformComponent>())
					{
						if (ImGui::Selectable(comp.c_str(), selected))
							entity->AddComponent<TransformComponent>();
					}
					if (comp == "MeshComponent" && !entity->Has<MeshComponent>())
					{
						if (ImGui::Selectable(comp.c_str(), selected))
							entity->AddComponent<MeshComponent>();
					}
					if (comp == "DirLightComponent" && !entity->Has<DirLightComponent>() && !entity->Has<PointLightComponent>())
					{
						if (ImGui::Selectable(comp.c_str(), selected))
							entity->AddComponent<DirLightComponent>();
					}
					if (comp == "PointLightComponent" && !entity->Has<DirLightComponent>() && !entity->Has<PointLightComponent>())
					{
						if (ImGui::Selectable(comp.c_str(), selected))
							entity->AddComponent<PointLightComponent>();
					}
					//if (selected)
					//	ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Separator();

			//static char strName[128];
			//ImGui::InputText("Object Name", strName, IM_ARRAYSIZE(strName),0,0,(void*)entity->m_Name.c_str());
			//entity->m_Name = strName;

			if (entity->Has<TransformComponent>() && !sceneSelected)
				ShowTransformComp(*entity);

			if (entity->Has<DirLightComponent>())
				ShowDirLightComp(*entity);

			if (entity->Has<PointLightComponent>())
				ShowPointLightComp(*entity);

			if (entity->Has<MeshComponent>())
				ShowMeshComp(*entity);
		}
		else
		{
			ImGui::Spacing();
			ImGui::Text("Multiple objects selected!");
		}

		if (sceneSelected)
		{
			//ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
			//ImGui::ColorEdit3("Quad Color", (float*)&quadColor);
		}
		ImGui::End();
	}

	void UiLayer::ShowTransformComp(Entity& entity)
	{
		ImGui::Spacing();
		ImGui::Text("Transform Component");
		ImGui::Spacing();
		glm::vec3 pos = TransformSystem::GetPosition(entity);
		glm::vec3 rot = TransformSystem::GetRotation(entity);
		glm::vec3 scale = TransformSystem::GetScale(entity);

		ImGui::DragFloat3("Position", (float*)&pos, 0.01f, -100, 100);
		ImGui::DragFloat3("Rotation", (float*)&rot, 0.1f, -179, 180);
		ImGui::DragFloat3("Scale", (float*)&scale, 0.01f, -100, 100);

		TransformSystem::SetPosition(entity, pos);
		TransformSystem::SetRotation(entity, rot);
		TransformSystem::SetScale(entity, scale);

		if (entity.HasParent())
		{
			ImGui::Text("Parent:");
			if (ImGui::Button(entity.GetParent().m_Name.c_str()))
			{
				entityToSelect = &entity.GetParent();
			}
			ImGui::SameLine();
			if (ImGui::Button("Unparent"))
			{
				entity.Unparent();
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
	}

	void UiLayer::ShowMeshComp(Entity& entity)
	{
		MeshComponent& mesh = entity.GetComponent<MeshComponent>();

		ImGui::Spacing();
		ImGui::Text("Mesh Component");
		ImGui::Spacing();

		auto meshIDs = ModelLibrary::GetInstance().GetMeshList();
		if (meshIDs.empty())
			return;

		if (ImGui::BeginCombo("Mesh", mesh.m_meshID.c_str()))
		{
			for (std::string meshID : meshIDs)
			{
				bool selected = (mesh.m_meshID == meshID);
				if (ImGui::Selectable(meshID.c_str(), selected))
					mesh.m_meshID = meshID;
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto materialIDs = ModelLibrary::GetInstance().GetMaterialList();
		if (ImGui::BeginCombo("Material", mesh.m_materialID.c_str()))
		{
			for (std::string matID : materialIDs)
			{
				bool selected = (mesh.m_materialID == matID);
				if (ImGui::Selectable(matID.c_str(), selected))
					mesh.m_materialID = matID;
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto shaderIDs = ModelLibrary::GetInstance().GetShaderList();
		if (ImGui::BeginCombo("Shader", mesh.m_shaderID.c_str()))
		{
			for (std::string shaderID : shaderIDs)
			{
				bool selected = (mesh.m_shaderID == shaderID);
				if (ImGui::Selectable(shaderID.c_str(), selected))
					mesh.m_shaderID = shaderID;
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		Material& material = ModelLibrary::GetInstance().GetMaterial(mesh.m_materialID);
		ImGui::DragFloat("Shininess", &material.shininess, 0.005f, 0.0f, 128.0f);
		if (material.diffuseTextures.empty())
		{
			ImGui::ColorPicker3("Color", (float*)&material.diffuseColor);
		}
		else
		{
			for (Tex& t : material.diffuseTextures)
			{
				ImGui::PushID(t.id);
				ImGui::Text(t.type.c_str());
				ImGui::Image((void*)t.id, ImVec2(128, 128));
				ImGui::PopID();
			}
		}
		if (!material.specularTextures.empty())
		{
			for (Tex& t : material.specularTextures)
			{
				ImGui::PushID(t.id);
				ImGui::Text(t.type.c_str());
				ImGui::Image((void*)t.id, ImVec2(128, 128));
				ImGui::PopID();
			}
		}

		if (ImGui::Button("Remove"))
		{
			mesh.m_Entity->RemoveComponent<MeshComponent>();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
	}

	void UiLayer::ShowDirLightComp(Entity& entity)
	{
		DirLightComponent& dirLight = entity.GetComponent<DirLightComponent>();

		ImGui::Spacing();
		ImGui::Text("Directional Light Component");
		ImGui::Spacing();

		ImGui::DragFloat3("Direction", (float*)&dirLight.m_direction, 0.01f, -1.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", (float*)&dirLight.m_ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&dirLight.m_diffuse);
		ImGui::ColorEdit3("Specular", (float*)&dirLight.m_specular);

		if (ImGui::Button("Remove"))
		{
			dirLight.m_Entity->RemoveComponent<DirLightComponent>();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
	}

	void UiLayer::ShowPointLightComp(Entity& entity)
	{
		PointLightComponent& pointLight = entity.GetComponent<PointLightComponent>();

		ImGui::Spacing();
		ImGui::Text("Point Light Component");
		ImGui::Spacing();

		ImGui::ColorEdit3("Ambient", (float*)&pointLight.m_ambient);
		ImGui::ColorEdit3("Diffuse", (float*)&pointLight.m_diffuse);
		ImGui::ColorEdit3("Specular", (float*)&pointLight.m_specular);
		ImGui::DragFloat("Constant", &pointLight.m_constant, 0.005f, 0.0f, 2.0f);
		ImGui::DragFloat("Linear", &pointLight.m_linear, 0.005f, 0.0f, 2.0f);
		ImGui::DragFloat("Quadratic", &pointLight.m_quadratic, 0.005f, 0.0f, 2.0f);

		if (ImGui::Button("Remove"))
		{
			pointLight.m_Entity->RemoveComponent<PointLightComponent>();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

	}

	void UiLayer::SetupAssetsWindow()
	{
		ImGui::Begin("Assets", &showObjectWindow);
		std::string path = "SleepyEngine\\Assets";
		path = _SOLUTIONDIR + path;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string fileName = entry.path().string().substr(entry.path().string().find_last_of("\\") + 1, entry.path().string().size());

			if (entry.is_directory())
			{
				fileName = ICON_FA_FOLDER_CLOSED + std::string(" ") + fileName;
			}
			else if (entry.is_regular_file())
			{

			}
			if (ImGui::Selectable(fileName.c_str()))
			{
				if (fileName.find(".") != std::string::npos)
				{
					ModelLibrary::GetInstance().AddMesh(entry.path().string());
				}
			}
		}
		ImGui::End();
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
}