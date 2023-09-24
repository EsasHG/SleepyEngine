#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <map>
namespace Sleepy
{
	struct BufferData
	{
		unsigned int framebuffer;
		unsigned int renderbuffer;
		unsigned int renderedTexture;
		unsigned int bufferHeigth = 1920;
		unsigned int bufferWidth = 1080;
	};

	class TransformComponent;
	class Renderer
	{
	public:
		Renderer(glm::vec2 windowSize);
		~Renderer();
		void BeginFrame(glm::vec2 windowSize);
		void PrepDraw(double deltaTime);
		static void DrawMesh(class MeshComponent mesh, TransformComponent transform);
		void DrawCubemap();
		unsigned int EndFrame();
		void DrawSceneTextureOnScreen(int texture);

		static void SetPointLightValues(unsigned int shaderID, TransformComponent& transform, class PointLightComponent& pointLight);
		static void SetDirLightValues(unsigned int shaderID, TransformComponent& transform, class DirLightComponent& pointLight);
#ifdef _DEBUG
		static void CheckGLError(std::string placeMessage);
#endif // DEBUG
		static unsigned int CreateShader(const char* vertShaderPath, const char* fragShaderPath);
		static unsigned int CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath);

		static void SetShaderUniformFloat(unsigned int m_ShaderId, const char* name, float f);
		static void SetShaderUniformInt(unsigned int m_ShaderId, const char* name, int i);
		static void SetShaderUniformBool(unsigned int m_ShaderId, const char* name, bool b);

		static void SetShaderUniformMat4(unsigned int m_ShaderId, const char* name, glm::mat4 matrix);
		void SetShaderUniformMat4(const char* name, glm::mat4 matrix);

		BufferData& AddFramebuffer();
		void SetFramebuffer(int id);
		void RecreateFramebuffer(BufferData& data);

		static void SetShaderUniformVec3(unsigned int m_ShaderId, const char* name, glm::vec3 vector);

		static void SetShaderUniformVec2(unsigned int m_ShaderId, const char* name, glm::vec2 vector);

		//TODO: Should not be here
		void SetCamera(class CameraComponent* camera);

	private:

		class Mesh* screenQuadMesh = nullptr;

		unsigned int m_ShaderId;
		unsigned int m_TextureShaderId;
		unsigned int m_SkyboxShaderId;
		unsigned int VAO, VBO;
		unsigned int skyboxVAO, skyboxVBO;	//TODO: Remove, should be somewhere else

		unsigned int boundBuffer = 0;

		std::vector<BufferData> frameBuffers;

		//TODO: Should not be here
		class CameraComponent* m_camera;

	};
}
	