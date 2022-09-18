#pragma once
#include <glm/glm.hpp>

class Window;
class UiLayer
{
public:
	UiLayer();
	void BeginFrame();
	void Run(double deltaTime);
	void EndFrame();
	glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
	glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);
	unsigned int sceneTexture;
	glm::vec2 contentRegionSize;
private:

	bool showTestWindow1 = true;
	bool showTestWindow2 = true;
	bool showRenderWindow = true;
	bool showTestWindow3 = true;


};
	static char inputText[128];

