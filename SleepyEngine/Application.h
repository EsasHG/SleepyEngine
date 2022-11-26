#pragma once



class Application
{
public:
	Application() {};
	~Application();
	double BeginFrame();
	int Run();
	void EndFrame();

	void FramebufferResizeCallback(int x, int y);
private:

	class UiLayer* ui;

	double prevFrameTime;
	class Window* window;
	class Renderer* renderer;
};

