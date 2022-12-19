#pragma once

#include <vector>
namespace Sleepy
{

	class Scene;
	class Application
	{
	public:
		Application();
		~Application();
		double BeginFrame();
		int Run();
		void EndFrame();
		void SetScene(Scene* scene)
		{
			m_scenes.clear();
			m_scenes.push_back(scene);
		}
		void FramebufferResizeCallback(int x, int y);
		static bool s_Playing;

	private:

		class UiLayer* ui;
		bool firstGameFrame = true;
		std::vector<Scene*> m_scenes;
		double prevFrameTime = 0;
		class Window* window;
		class Renderer* renderer;
	};


}