#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Components/InputComponent.h"
#include "Components/CameraComponent.h"

namespace Sleepy
{
	Camera::Camera(std::string name, class Sleepy::SceneBase* scene) : Sleepy::Entity(name, scene)
	{
		m_Camera = &AddComponent<CameraComponent>();

		m_Input = &AddComponent<InputComponent>();
		m_Input->AddKeyBinding(GLFW_KEY_W, SLE_HELD, std::bind(&Camera::MoveForward, this));
		m_Input->AddKeyBinding(GLFW_KEY_S, SLE_HELD, std::bind(&Camera::MoveBackward, this));
		m_Input->AddKeyBinding(GLFW_KEY_A, SLE_HELD, std::bind(&Camera::MoveLeft, this));
		m_Input->AddKeyBinding(GLFW_KEY_D, SLE_HELD, std::bind(&Camera::MoveRight, this));
		m_Input->AddKeyBinding(GLFW_KEY_Q, SLE_HELD, std::bind(&Camera::MoveDown, this));
		m_Input->AddKeyBinding(GLFW_KEY_E, SLE_HELD, std::bind(&Camera::MoveUp, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_PRESSED, std::bind(&Camera::MouseButtonPressed, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_RELEASED, std::bind(&Camera::MouseButtonReleased, this));
		m_Input->AddMousePosBinding(std::bind(&Camera::CursorMoveCallback, this, std::placeholders::_1, std::placeholders::_2));
		SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));
		m_Camera->UpdateVectors();
	}
	
	Camera::~Camera()
	{
		delete m_Input;
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
			m_Camera->yaw += xoffset;
			m_Camera->pitch += yoffset;
	
			if (m_Camera->pitch > 89.0f)
				m_Camera->pitch = 89.0f;
			if (m_Camera->pitch < -89.0f)
				m_Camera->pitch = -89.0f;
	
			m_Camera->UpdateVectors();
		}
	
		//std::cout << "xPos: " << xPos << "yPos: " << yPos << std::endl;
	}
	
	void Camera::MoveForward()
	{
		SetPosition(GetPosition() + m_Camera->front * frameCameraSpeed);
	}
	
	void Camera::MoveBackward()
	{
		SetPosition(GetPosition() - m_Camera->front * frameCameraSpeed);
	}
	
	void Camera::MoveLeft()
	{
		SetPosition(GetPosition() - m_Camera->right * frameCameraSpeed);
	}
	
	void Camera::MoveRight()
	{
		SetPosition(GetPosition() + m_Camera->right * frameCameraSpeed);
	}
	
	void Camera::MoveDown()
	{
		SetPosition(GetPosition() - m_Camera->up * frameCameraSpeed);
	}
	
	void Camera::MoveUp()
	{
		SetPosition(GetPosition() + m_Camera->up * frameCameraSpeed);
	}
	
	void Camera::MouseButtonPressed()
	{
		mousePressed = true;
	}
	void Camera::MouseButtonReleased()
	{
		mousePressed = false;
	}
}