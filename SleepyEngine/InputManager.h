#pragma once
#include <vector>
#include "Components/InputComponent.h"
struct GLFWwindow;

static Sleepy::InputManager* s_InputManager;
namespace Sleepy
{
	class InputManager
	{
	public:
		static InputManager& GetInstance() {
			if(!s_InputManager)
				s_InputManager = new InputManager();
			return *s_InputManager;
		}
	
		void RunEvents();

		void RunEvents(InputComponent& input);
	
		void Init(GLFWwindow* window);
		void AddWindowResizeCallback(std::function<void(int, int)> func);
	
		void RemoveInputComponent(InputComponent* input);
	private:
		InputManager() {};
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		void FramebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);
	
	
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void KeyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	
		static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
		void CursorPosCallbackImpl(GLFWwindow* window, double xPos, double yPos);
	
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods);

		static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		void MouseScrollCallbackImpl(GLFWwindow* window, double xOffset, double yOffset);
	
		std::vector<InputComponent*> m_Inputs;
		std::vector<std::function<void(int, int)>> m_FramebufferSizeCallbacks;
		bool bInitialized = false;
	
		bool bDispatchKeyboardEvents = true;
		bool bDispatchMouseEvents = true;
	
		void AddInputComponent(InputComponent* i);
		friend class InputComponent;
	};
}