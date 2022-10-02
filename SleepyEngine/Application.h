#pragma once



class Application
{
public:
	Application() {};
	~Application();
	double BeginFrame();
	int Run();
	void EndFrame();
private:

	double prevFrameTime;
	class Window* window;
	class Renderer* renderer;
};

