#pragma once
class Application
{
public:
	Application() {};
	int Run();
	void TestFunc();

	void TestFunc3(double xPos, double yPos);
	void MouseButtonPressed();

	//TEMP
	bool firstMouse = true;
	bool mousePressed = false;
	double lastX, lastY;
	float yaw = -90;
	float pitch = 0;
};

