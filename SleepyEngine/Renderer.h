#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <map>

#include "Components/CameraComponent.h"
namespace Sleepy
{
	//struct BufferData
	//{
	//	unsigned int framebuffer;
	//	unsigned int renderbuffer;
	//	unsigned int renderedTexture;
	//	unsigned int bufferHeigth = 1920;
	//	unsigned int bufferWidth = 1080;
	//};

	class TransformComponent;
	class Renderer
	{
	public:
		Renderer(glm::vec2 windowSize);
		~Renderer();
		void BeginFrame();
		void PrepDraw(CameraComponent& camera);
		static void DrawMesh(class MeshComponent mesh, TransformComponent transform);
		void DrawCubemap();
		unsigned int EndBufferRender();
		void DrawSceneTextureOnScreen(int texture);

		//BufferData& AddFramebuffer();
		//void SetFramebuffer(int id);
		static void RecreateFramebuffer(BufferData& data);

		//TODO: Should not be here
		//void SetCamera(class CameraComponent* camera);

#ifdef _DEBUG
		static void CheckGLError(std::string placeMessage);
#endif // DEBUG
		static unsigned int CreateShader(const char* vertShaderPath, const char* fragShaderPath);
		static unsigned int CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath);

		static void SetPointLightValues(unsigned int shaderID, TransformComponent& transform, class PointLightComponent& pointLight);
		static void SetDirLightValues(unsigned int shaderID, TransformComponent& transform, class DirLightComponent& pointLight);

		static void SetShaderUniformFloat(unsigned int m_ShaderId, const char* name, float f);
		static void SetShaderUniformInt(unsigned int m_ShaderId, const char* name, int i);
		static void SetShaderUniformBool(unsigned int m_ShaderId, const char* name, bool b);

		static void SetShaderUniformMat4(unsigned int m_ShaderId, const char* name, glm::mat4 matrix);
		//void SetShaderUniformMat4(const char* name, glm::mat4 matrix);

		static void SetShaderUniformVec2(unsigned int m_ShaderId, const char* name, glm::vec2 vector);

		static void SetShaderUniformVec3(unsigned int m_ShaderId, const char* name, glm::vec3 vector);



	private:

		class Mesh* screenQuadMesh = nullptr;

		unsigned int m_ShaderId;
		unsigned int m_TextureShaderId;
		unsigned int m_SkyboxShaderId;
		unsigned int m_VertexColorShaderId;
		unsigned int VAO, VBO;
		unsigned int skyboxVAO, skyboxVBO;	//TODO: Remove, should be somewhere else
		unsigned int usedBuffers;
		unsigned int boundBuffer = 0;

		std::vector<BufferData> frameBuffers;

		//TODO: Should not be here
		//class CameraComponent* m_camera;

	};
}
	