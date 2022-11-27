#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <map>

class MeshComponent;
class TransformComponent;
class Renderer
{
public:
	Renderer(glm::vec2 windowSize);
	~Renderer();
	void BeginFrame(glm::vec2 windowSize);
	unsigned int Draw(double deltaTime);
	static void DrawMesh(MeshComponent mesh, TransformComponent transform);

	static void SetPointLightValues(unsigned int shaderID, TransformComponent& transform, class PointLightComponent& pointLight);
	static void SetDirLightValues(unsigned int shaderID, TransformComponent& transform, class DirLightComponent& pointLight);
	void EndFrame();
	static void CheckGLError(std::string placeMessage);
	static unsigned int CreateShader(const char* vertShaderPath, const char* fragShaderPath);
	static unsigned int CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath);

	static void SetShaderUniformFloat(unsigned int m_ShaderId, const char* name, float f);

	static void SetShaderUniformInt(unsigned int m_ShaderId, const char* name, int i);

	static void SetShaderUniformMat4(unsigned int m_ShaderId, const char* name, glm::mat4 matrix);
	void SetShaderUniformMat4( const char* name, glm::mat4 matrix);

	void RecreateFramebuffer();

	static void SetShaderUniformVec3(unsigned int m_ShaderId, const char* name, glm::vec3 vector);

	static void SetShaderUniformVec2(unsigned int m_ShaderId, const char* name, glm::vec2 vector);

	//TODO: Should not be here
	void SetCamera(class Camera* camera);
	void ResizeViewport(int x, int y);

private:


	glm::vec2 m_WindowSize;

	unsigned int m_ShaderId;
	unsigned int m_QuadShaderId;
	unsigned int m_TextureShaderId;
	unsigned int VAO, VBO;
	unsigned int FBO;
	unsigned int RBO;

	unsigned int renderedTexture;

	//TODO: Should not be here
	class Camera* m_camera;

};

