#pragma once

#include <glm/glm.hpp>
#include "Entity.h"

namespace Sleepy
{
	class InputComponent;
	class Camera : public Entity
	{
	public:
		Camera(std::string name, class Sleepy::SceneBase* scene);
		~Camera();
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
	
		void MouseButtonPressed();
		void MouseButtonReleased();
	
		bool firstMouse = true;
		bool mousePressed = false;
		double lastX, lastY;
		float xoffset;
		float yoffset;
	
		float cameraSpeed = 5.0;
		glm::vec3 moveVector = glm::vec3(0.0f);

		InputComponent* m_Input;
	};
}