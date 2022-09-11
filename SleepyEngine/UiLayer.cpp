#include "UiLayer.h"

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
	}

}

void UiLayer::Run()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

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
		ImGui::Text("Testing a window");
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
