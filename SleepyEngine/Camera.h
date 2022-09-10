#pragma once

#include <glm/glm.hpp>

class Input;
class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 GetViewMatrix();
	void Run(double deltaTime);
private:
	void CursorMoveCallback(double xPos, double yPos);
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MouseButtonPressed();
	void MouseButtonReleased();

	void UpdateVectors();

	bool firstMouse = true;
	bool mousePressed = false;
	double lastX, lastY;
	float yaw = -90;
	float pitch = 0;
	float xoffset;
	float yoffset;

	float frameCameraSpeed;

	float cameraSpeed = 5.0;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	Input* m_Input;
};

