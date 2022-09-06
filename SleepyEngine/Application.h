#pragma once
class Application
{
public:
	Application() {};
	int Run();

private:
	void MoveLeft();
	void MoveRight();
	void TestFunc3(double xPos, double yPos);
	void MouseButtonPressed();

	//TEMP
	bool firstMouse = true;
	bool mousePressed = false;
	double lastX, lastY;
	float yaw = -90;
	float pitch = 0;
	float xoffset;
	float yoffset;

	float cameraSpeed = 0.03;
};

