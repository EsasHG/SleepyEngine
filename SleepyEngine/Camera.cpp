#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"


namespace Sleepy
{
	Camera::Camera()
	{
		m_Input = new Input();
		m_Input->AddKeyBinding(GLFW_KEY_W, SLE_HELD, std::bind(&Camera::MoveForward, this));
		m_Input->AddKeyBinding(GLFW_KEY_S, SLE_HELD, std::bind(&Camera::MoveBackward, this));
		m_Input->AddKeyBinding(GLFW_KEY_A, SLE_HELD, std::bind(&Camera::MoveLeft, this));
		m_Input->AddKeyBinding(GLFW_KEY_D, SLE_HELD, std::bind(&Camera::MoveRight, this));
		m_Input->AddKeyBinding(GLFW_KEY_Q, SLE_HELD, std::bind(&Camera::MoveDown, this));
		m_Input->AddKeyBinding(GLFW_KEY_E, SLE_HELD, std::bind(&Camera::MoveUp, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_PRESSED, std::bind(&Camera::MouseButtonPressed, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_RELEASED, std::bind(&Camera::MouseButtonReleased, this));
		m_Input->AddMousePosBinding(std::bind(&Camera::CursorMoveCallback, this, std::placeholders::_1, std::placeholders::_2));
	
		UpdateVectors();
	}
	
	Camera::~Camera()
	{
		delete m_Input;
	}
	
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}
	
	const glm::vec3 Camera::GetPosition() 
	{
		return position; 
	}
	
	void Camera::Run(double deltaTime)
	{
		frameCameraSpeed = cameraSpeed * deltaTime;
	}
	
	
	void Camera::CursorMoveCallback(double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}
	
		xoffset = xPos - lastX;
		yoffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;
	
	
		if (mousePressed)
		{
			const float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			//std::cout << "x offset " << xoffset << "y offset " << yoffset << std::endl;
			yaw += xoffset;
			pitch += yoffset;
	
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
	
			UpdateVectors();
		}
	
		//std::cout << "xPos: " << xPos << "yPos: " << yPos << std::endl;
	}
	
	void Camera::MoveForward()
	{
		position += front * frameCameraSpeed;
	}
	
	void Camera::MoveBackward()
	{
		position -= front * frameCameraSpeed;
	}
	
	void Camera::MoveLeft()
	{
		position -= right * frameCameraSpeed;
	}
	
	void Camera::MoveRight()
	{
		position += right * frameCameraSpeed;
	}
	
	void Camera::MoveDown()
	{
		position -= up * frameCameraSpeed;
	
	}
	
	void Camera::MoveUp()
	{
		position += up * frameCameraSpeed;
	
	}
	
	void Camera::MouseButtonPressed()
	{
		mousePressed = true;
	}
	void Camera::MouseButtonReleased()
	{
		mousePressed = false;
	}
	
	void Camera::UpdateVectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(direction);
	
		right = glm::normalize(glm::cross(front, up));
	}

}