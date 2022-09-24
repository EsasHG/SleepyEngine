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
		ImGui::SliderFloat3("Point Light Pos", (float*)&pointLightPos,-100.0f, 100.0f);
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
