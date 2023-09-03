#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include <vector>

#include "Component.h"

namespace Sleepy
{
	enum SLE_INPUT_MODE
	{
		SLE_PRESSED,
		SLE_RELEASED,
		SLE_HELD
	};
	
	
	class InputComponent : public Component
	{
	public:
		InputComponent(Entity* entity);
		~InputComponent();
		void AddKeyBinding(int key, SLE_INPUT_MODE mode, std::function<void()> func);
		void AddMousePosBinding(std::function<void(double, double)> func);
		void AddMouseButtonBinding(int button, SLE_INPUT_MODE mode, std::function<void()> func);
	private:
		void RunKeyEvents();
		void RunMouseEvents();
	
		void HandleKeyEvents(int key, int action);
		void HandleMousePosEvents(double xPos, double yPos);
		void HandleMouseButtonEvents(int button, int action);
	
		std::vector<int> m_ActiveKeys;
		std::vector<int> m_ActiveMouseButtons;
	
		std::map<int, std::vector<std::function<void()>>> m_HeldKeyCallbacks;
		std::map<int, std::vector<std::function<void()>>> m_PressedKeyCallbacks;
		std::map<int, std::vector<std::function<void()>>> m_ReleasedKeyCallbacks;
	
		std::map<int, std::vector<std::function<void()>>> m_HeldMouseButtonCallbacks;
		std::map<int, std::vector<std::function<void()>>> m_PressedMouseButtonCallbacks;
		std::map<int, std::vector<std::function<void()>>> m_ReleasedMouseButtonCallbacks;
	
		std::vector<std::function<void(double, double)>> m_MousePosCallbacks;
		friend class InputManager;
	};
}

	