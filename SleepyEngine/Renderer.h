#pragma once

#include <glm/glm.hpp>
class Renderer
{
public:
	Renderer(glm::vec2 windowSize);
	void Draw(double deltaTime);
	static unsigned int CreateShader(const char* vertShaderPath, const char* fragShaderPath);
	static unsigned int CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath);

	static void SetShaderUniformFloat(unsigned int m_ShaderId, const char* name, float f);

	static void SetShaderUniformInt(unsigned int m_ShaderId, const char* name, int i);

	static void SetShaderUniformMat4(unsigned int m_ShaderId, const char* name, glm::mat4 matrix);
	void SetShaderUniformMat4( const char* name, glm::mat4 matrix);

	void RecreateFramebuffer();

	static void SetShaderUniformVec3(unsigned int m_ShaderId, const char* name, glm::vec3 vector);

	static void SetShaderUniformVec2(unsigned int m_ShaderId, const char* name, glm::vec2 vector);
private:
	void FramebufferResizeCallback(int x, int y);

	class UiLayer* ui;

	glm::vec2 m_WindowSize;

	unsigned int m_ShaderId;
	unsigned int m_QuadShaderId;
	unsigned int VAO, VBO;
	unsigned int FBO;
	unsigned int renderedTexture;

};

