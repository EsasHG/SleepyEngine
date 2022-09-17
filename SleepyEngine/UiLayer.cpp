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
		//style.WindowPadding = ImVec2(0.0f, 0.0f);

	}
}

void UiLayer::Run(double deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiDockNodeFlags docking_Flags = 0;//ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),docking_Flags);
	ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
	
	bool windowFocused = false;

	if (showRenderWindow)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		//window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		window_flags |= ImGuiWindowFlags_NoInputs;
		ImGui::Begin("Game Window", &showRenderWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNavInputs);
		ImVec2 pos = ImGui::GetCursorScreenPos();
		//ImGui::BeginChild("GameRender",ImVec2(0,0), window_flags);
		ImVec2 crSize = ImGui::GetContentRegionAvail();
		//ImVec2 wSize = ImGui::GetWindowSize();
		//if (ImGui::ImageButton((ImTextureID)sceneTexture, ImVec2(crSize.x - 10, crSize.y - 10), ImVec2(0, 1), ImVec2(1, 0)))
		//ImGui::GetWindowDrawList()->AddImage((ImTextureID)sceneTexture, pos, ImVec2(pos.x + crSize.y / 2, pos.x + crSize.y / 2), ImVec2(0, 1), ImVec2(1, 0));
		
		
		ImGui::Image((ImTextureID)sceneTexture, crSize, ImVec2(0, 1), ImVec2(1, 0));
		//ImGuiFocusedFlags_

		if (ImGui::IsWindowFocused())
		{
			windowFocused = true;
			ImGui::SetNextFrameWantCaptureMouse(false);
			ImGui::SetNextFrameWantCaptureKeyboard(false);
			//ImGui::CaptureKeyboardFromApp(false);
		}
			//ImGui::SetWindowFocus(NULL);
		contentRegionSize = glm::vec2(crSize.x, crSize.y);
		//ImGui::Image((ImTextureID)sceneTexture, wSize, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::EndChild();
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
		//ImGui::InputText("Test field")
		ImGui::Checkbox("Show Window 2", &showTestWindow2);
		ImGui::End();
	}

	if (showTestWindow2)
	{
		ImGui::Begin("ImGui Window 2 !", &showTestWindow2);
		ImGui::Text("Testing a window 2 ");
		//ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
		//ImGui::ColorEdit3("Quad Color", (float*)&quadColor);
		ImGui::InputText("Test field", inputText, IM_ARRAYSIZE(inputText));

		ImGui::End();
	}


	
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
