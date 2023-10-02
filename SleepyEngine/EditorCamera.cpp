#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Components/InputComponent.h"
#include "Components/CameraComponent.h"

namespace Sleepy
{
	EditorCamera::EditorCamera(std::string name, class Sleepy::SceneBase* scene) : Sleepy::Entity(name, scene)
	{

		m_Input = &AddComponent<InputComponent>();
		m_Input->AddKeyBinding(GLFW_KEY_W, SLE_HELD, std::bind(&EditorCamera::MoveForward, this));
		m_Input->AddKeyBinding(GLFW_KEY_S, SLE_HELD, std::bind(&EditorCamera::MoveBackward, this));
		m_Input->AddKeyBinding(GLFW_KEY_A, SLE_HELD, std::bind(&EditorCamera::MoveLeft, this));
		m_Input->AddKeyBinding(GLFW_KEY_D, SLE_HELD, std::bind(&EditorCamera::MoveRight, this));
		m_Input->AddKeyBinding(GLFW_KEY_Q, SLE_HELD, std::bind(&EditorCamera::MoveDown, this));
		m_Input->AddKeyBinding(GLFW_KEY_E, SLE_HELD, std::bind(&EditorCamera::MoveUp, this));

		m_Input->AddKeyBinding(GLFW_KEY_LEFT_SHIFT, SLE_PRESSED, std::bind(&EditorCamera::ShiftPressed, this));
		m_Input->AddKeyBinding(GLFW_KEY_LEFT_SHIFT, SLE_RELEASED, std::bind(&EditorCamera::ShiftReleased, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_PRESSED, std::bind(&EditorCamera::MouseButtonPressed, this));
		m_Input->AddMouseButtonBinding(GLFW_MOUSE_BUTTON_2, SLE_RELEASED, std::bind(&EditorCamera::MouseButtonReleased, this));
		m_Input->AddMouseScrollBinding(std::bind(&EditorCamera::MouseScrolled, this, std::placeholders::_1, std::placeholders::_2));

		m_Input->AddMousePosBinding(std::bind(&EditorCamera::CursorMoveCallback, this, std::placeholders::_1, std::placeholders::_2));
		SetPosition(glm::vec3(0.0f, 0.0f, 4.0f));

		m_Camera = &AddComponent<CameraComponent>();
		m_Camera->bPossessOnStart = false;
		m_Camera->UpdateVectors();

		enableUpdate = false;
	}
	
	EditorCamera::~EditorCamera()
	{
		delete m_Input;
	}
	
	void EditorCamera::Update(double deltaTime)
	{
		Entity::Update(deltaTime);

		float frameCameraSpeed = cameraSpeed * deltaTime;
		glm::normalize(moveVector);
		SetPosition(GetPosition() + moveVector * frameCameraSpeed);
		moveVector = glm::vec3(0.0f);
	}
	
	void EditorCamera::CursorMoveCallback(double xPos, double yPos)
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
			
			glm::vec3 rot = GetRotation();

			rot.x += xoffset;
			rot.y += yoffset;

			if (rot.y > 89.0f)
				rot.y = 89.0f;
			if (rot.y < -89.0f)
				rot.y = -89.0f;

			SetRotation(rot);
	
			m_Camera->UpdateVectors();
		}
	}

	void EditorCamera::MouseScrolled(double xOffset, double yOffset)
	{
		m_Camera->fov += yOffset * zoomSpeed;
		m_Camera->fov = glm::clamp(m_Camera->fov, 0.025f, 1.0f);
	}

	void EditorCamera::MouseButtonPressed()
	{
		mousePressed = true;
	}

	void EditorCamera::MouseButtonReleased()
	{
		mousePressed = false;
	}

	void EditorCamera::MoveForward()
	{
		moveVector +=  m_Camera->front;
	}
	
	void EditorCamera::MoveBackward()
	{
		moveVector -= m_Camera->front;
	}
	
	void EditorCamera::MoveLeft()
	{
		moveVector -= m_Camera->right;
	}
	
	void EditorCamera::MoveRight()
	{
		moveVector += m_Camera->right;
	}
	
	void EditorCamera::MoveDown()
	{
		moveVector -= m_Camera->up;
	}
	
	void EditorCamera::MoveUp()
	{
		moveVector += m_Camera->up;
	}

	void EditorCamera::ShiftPressed()
	{
		cameraSpeed = superCameraSpeed;
	}

	void EditorCamera::ShiftReleased()
	{
		cameraSpeed = normalCameraSpeed;
	}
	

}