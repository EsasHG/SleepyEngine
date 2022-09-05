#pragma once

#include <glm/glm.hpp>
class Renderer
{
public:
	static unsigned int CreateShader(const char* vertShaderPath, const char* fragShaderPath);
	static unsigned int CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath);

	void SetShaderUniformFloat(unsigned int shaderId, const char* name, float f);

	void SetShaderUniformInt(unsigned int shaderId, const char* name, int i);

	static void SetShaderUniformMat4(unsigned int shaderId, const char* name, glm::mat4 matrix);

	static void SetShaderUniformVec3(unsigned int shaderId, const char* name, glm::vec3 vector);

	void SetShaderUniformVec2(unsigned int shaderId, const char* name, glm::vec2 vector);

};

