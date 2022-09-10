#pragma once
class Application
{
public:
	Application() {};
	int Run();

private:

	void WindowResizeCallback(int width, int height);

	int windowWidth = 800, windowHeight = 600;
	unsigned int shaderId;
	double prevFrameTime;
};

