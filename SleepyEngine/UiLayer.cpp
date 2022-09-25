#include "UiLayer.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

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

void UiLayer::Run(double deltaTime)
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

	if (showTestWindow1)
	{
		ImGui::Begin("ImGui Window!", &showTestWindow1);
		ImGui::Text("Testing a window");

		ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
		ImGui::ColorEdit3("Quad Color", (float*)&quadColor);
		if (pointLightSelected)
		{
			ImGui::SliderFloat3("Point Light Position", (float*)&pointLightPos, -10.0f, 10.0f);
			ImGui::ColorEdit3("Point Light Diffuse", (float*)&pointLightDiffuse);
		}
		if (dirLightSelected)
		{
			ImGui::InputFloat3("Dir Light Direction", (float*)&dirLightDir);// -10.0f, 10.0f);
			ImGui::ColorEdit3("Dir Light Diffuse", (float*)&dirLightDiffuse);
		}
		//ImGui::InputText("Test field")
		ImGui::Checkbox("Show Window 2", &showTestWindow2);
		ImGui::End();
	}

	if (showTestWindow2)
	{
		ImGui::Begin("Object tree!", &showTestWindow2);
		ImGui::BeginTable("Table test", 2);
		ImGui::PushID(1);
		ImGui::TableNextColumn();
		ImGui::TableSetColumnIndex(0);
		bool bOpen = ImGui::TreeNode("Object");
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static int selection_mask = (1 << 2);
		int node_clicked = -1;

		
		for (int i = 0; i < 2; i++)
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = (selection_mask & (1 << i)) != 0;
			if (is_selected)
			{
				node_flags |= ImGuiTreeNodeFlags_Selected;
				if (i == 0)
					pointLightSelected = true;
				else if (i == 1)
					dirLightSelected = true;

			}
			else
			{
				if (i == 0)
					pointLightSelected = false;
				else if (i == 1)
					dirLightSelected = false;
			}
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				node_clicked = i;
				pointLightSelected;
			}
			if (node_open)
			{
				ImGui::BulletText("Open");
				ImGui::TreePop();
			}
		}
		if (node_clicked != -1)
		{
			// Update selection state
			// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
			if (ImGui::GetIO().KeyCtrl)
				selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
			else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
				selection_mask = (1 << node_clicked);           // Click to single-select
		}
		


		//bool node_open = ImGui::TreeNodeEx("Directional Light", base_flags);
		//if (ImGui::IsItemClicked())
		//{
		//	node_clicked = 0;
		//	dirLightSelected;
		//}


		//bool node_open = ImGui::TreeNodeEx("Directional Light", base_flags);

		//ImGui::Selectable("Point Light", &pointLightSelected);
		//ImGui::Selectable("Directional Light", &dirLightSelected);
		//ImGui::TableSetColumnIndex(1);
		//if (bOpen)
		//{
		//	ImGui::TableNextRow();
		//	ImGui::TableSetColumnIndex(0); 
		//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
		//	//ImGui::TreeNodeEx("Field", flags);

		//	ImGui::TreePop();

		//}
		if(bOpen)
			ImGui::TreePop();

		ImGui::PopID();

		ImGui::EndTable();

		ImGui::End();
	}

	
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
