#include "Player.h"
#include <glm/gtx/norm.hpp>
void Player::BeginPlay()
{
	m_Input = new Sleepy::Input();
	m_Input->AddKeyBinding(GLFW_KEY_UP, Sleepy::SLE_HELD, std::bind(&Player::MoveForward, this));
	m_Input->AddKeyBinding(GLFW_KEY_DOWN, Sleepy::SLE_HELD, std::bind(&Player::MoveBackward, this));
	m_Input->AddKeyBinding(GLFW_KEY_LEFT, Sleepy::SLE_HELD, std::bind(&Player::MoveLeft, this));
	m_Input->AddKeyBinding(GLFW_KEY_RIGHT, Sleepy::SLE_HELD, std::bind(&Player::MoveRight, this));
}

Player::~Player()
{
	delete m_Input;
}

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
	velocity += glm::vec3(0.0f, 0.0f, -1.0f);
}

void Player::MoveBackward()
{
	velocity += glm::vec3(0.0f, 0.0f, 1.0f);
}

void Player::MoveLeft()
{
	velocity += glm::vec3(-1.0f, 0.0f, 0.0f);
}

void Player::MoveRight()
{
	velocity += glm::vec3(1.0f, 0.0f, 0.0f);
}