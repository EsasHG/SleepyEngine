#pragma once
#include <glm/glm.hpp>

class Window;
class UiLayer
{
public:
	UiLayer();
	void Run();
	glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
	glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);
private:

	bool showTestWindow1 = true;
	bool showTestWindow2 = false;

};
	static char inputText[128];

