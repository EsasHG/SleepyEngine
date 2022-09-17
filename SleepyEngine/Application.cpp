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

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Renderer.h"
#include "Input.h"
#include "InputManager.h"
#include "Camera.h"
#include "Window.h"
#include "UiLayer.h"

//TEMP

int Application::Run()
{


	Window window(800, 600, "Sleepy Engine");
	Renderer r(glm::vec2(window.GetWidth(), window.GetHeight()));
	window.EnableImGui();
	
	Camera camera;
	while (!window.ShouldClose())
	{
		double time = glfwGetTime();
		double deltaTime = time - prevFrameTime;
		prevFrameTime = time;
		auto io = ImGui::GetIO();
		//std::cout << io.WantCaptureKeyboard	<< "mouse: " << io.WantCaptureMouse << std::endl;
		
		glm::mat4 view = camera.GetViewMatrix();
		r.SetShaderUniformMat4("view", view);
		glfwPollEvents();

		if (!ImGui::GetIO().WantCaptureKeyboard)
			InputManager::GetInstance().RunEvents();
		camera.Run(deltaTime);

		r.Draw(deltaTime);
	
		window.SwapBuffers();
	}
	return 0;
}