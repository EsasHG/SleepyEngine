#pragma once

#include <glm/glm.hpp>
#include "Entity.h"

namespace Sleepy
{
	class InputComponent;
	class EditorCamera : public Entity
	{
	public:
		EditorCamera(std::string name, class Sleepy::SceneBase* scene);
		~EditorCamera();
		virtual void Update(double deltaTime) override;

		class CameraComponent* m_Camera;
	private:
		void CursorMoveCallback(double xPos, double yPos);
		void MoveForward();
		void MoveBackward();
		void MoveLeft();
		void MoveRight();
		void MoveDown();
		void MoveUp();
		void ShiftPressed();
		void ShiftReleased();

		void MouseButtonPressed();
		void MouseButtonReleased();
		void MouseScrolled(double xOffset, double yOffset);


		bool firstMouse = true;
		bool mousePressed = false;
		double lastX, lastY;
		float xoffset;
		float yoffset;
	
		float cameraSpeed = 5.0;
		float zoomSpeed = 0.01f;
		float normalCameraSpeed = 5.0;
		float superCameraSpeed = 20.0;
		glm::vec3 moveVector = glm::vec3(0.0f);

		InputComponent* m_Input;
	};
}