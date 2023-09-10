#include "Player.h"
#include <glm/gtx/norm.hpp>

#include <Components/CameraComponent.h>

void Player::BeginPlay()
{
	m_Input = &AddComponent<Sleepy::InputComponent>();
	m_Input->AddKeyBinding(GLFW_KEY_UP, Sleepy::SLE_HELD, std::bind(&Player::MoveForward, this));
	m_Input->AddKeyBinding(GLFW_KEY_DOWN, Sleepy::SLE_HELD, std::bind(&Player::MoveBackward, this));
	m_Input->AddKeyBinding(GLFW_KEY_LEFT, Sleepy::SLE_HELD, std::bind(&Player::MoveLeft, this));
	m_Input->AddKeyBinding(GLFW_KEY_RIGHT, Sleepy::SLE_HELD, std::bind(&Player::MoveRight, this));

	m_Input->AddMousePosBinding(std::bind(&Player::CursorMoveCallback, this, std::placeholders::_1, std::placeholders::_2));
}

//Player::~Player()
//{
//	Sleepy::Entity::~Entity();
//}

void Player::Update(double deltaTime)
{

	float border = 50.0f;
	glm::vec3 pos = GetPosition();
	if (pos.x < -border)
	{
		velocity.x = 0.0;
		pos.x = -(border-0.1);
	}
	if (pos.x > border)
	{
		velocity.x = 0.0;
		pos.x = (border - 0.1);
	}
	if (pos.y > border)
	{
		velocity.y = 0.0;
		pos.y = (border - 0.1);
	}
	if (pos.y < -border)
	{
		velocity.y = 0.0;
		pos.y = -(border - 0.1);
	}
	if (pos.z > border)
	{
		velocity.z = 0.0;
		pos.z = (border - 0.1);
	}
	if (pos.z < -border)
	{
		velocity.z = 0.0;
		pos.z = -(border - 0.1);
	}
	if (glm::length2(velocity) > maxSpeed * maxSpeed)
	{
		velocity = glm::normalize(velocity) * maxSpeed;
	}
		

	SetPosition(pos + (float(deltaTime) * velocity));

}

void Player::MoveForward()
{
	if (!m_Camera) return;
	velocity += glm::normalize(m_Camera->front);
}

void Player::MoveBackward()
{
	if (!m_Camera) return;
	velocity -= glm::normalize(m_Camera->front);
}

void Player::MoveLeft()
{
	if (!m_Camera) return;
	velocity -= glm::normalize(m_Camera->right);
}

void Player::MoveRight()
{
	if (!m_Camera) return;
	velocity += glm::normalize(m_Camera->right);
}

void Player::CursorMoveCallback(double xPos, double yPos)
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


	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::vec3 rot = GetRotation();

	rot.x += yoffset;
	rot.y += xoffset;

	if (rot.y > 89.0f)
		rot.y = 89.0f;
	if (rot.y < -89.0f)
		rot.y = -89.0f;

	SetRotation(rot);
	if (!m_Camera)
	{
		auto children = GetChildren();
		if(children.size()>0)
		{
			for (auto child : children)
			{
				if (child->Has<Sleepy::CameraComponent>())
				{
					m_Camera = &child->GetComponent<Sleepy::CameraComponent>();
					m_Camera->UpdateVectors();
					break;
				}
			}
		}
	}
	else
	{
		m_Camera->UpdateVectors();
	}

}