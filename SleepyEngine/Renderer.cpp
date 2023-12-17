#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>
#include "InputManager.h"
#include "Model.h"
#include "ModelLibrary.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/CameraComponent.h"
#include "TransformSystem.h"
#include "RenderTarget.h"

namespace Sleepy
{

	Renderer::Renderer(glm::vec2 windowSize)
	{
		if (!gladLoadGL())
		{
			std::cout << "Glad did not load successfully!" << std::endl;
		}

		m_ShaderId = Renderer::CreateShader(_SOLUTIONDIR"SleepyEngine/Shaders/SingleColor.vert", _SOLUTIONDIR"SleepyEngine/Shaders/SingleColor.frag");
		ModelLibrary::GetInstance().AddShader("color", m_ShaderId);
		m_TextureShaderId = Renderer::CreateShader(_SOLUTIONDIR"SleepyEngine/Shaders/TextureShader.vert", _SOLUTIONDIR"SleepyEngine/Shaders/TextureShader.frag");

		m_SkyboxShaderId = Renderer::CreateShader(_SOLUTIONDIR"SleepyEngine/Shaders/Skybox.vert", _SOLUTIONDIR"SleepyEngine/Shaders/Skybox.frag");

		ModelLibrary::GetInstance().AddShader("default", m_TextureShaderId);
		ModelLibrary::GetInstance().AddShader("quadShader", Renderer::CreateShader(_SOLUTIONDIR"SleepyEngine/Shaders\\QuadShader.vert", _SOLUTIONDIR"SleepyEngine/Shaders\\QuadShader.frag"));

		m_VertexColorShaderId = Renderer::CreateShader(_SOLUTIONDIR"SleepyEngine/Shaders/VertexColor.vert", _SOLUTIONDIR"SleepyEngine/Shaders/VertexColor.frag");
		ModelLibrary::GetInstance().AddShader("vertexColor", m_VertexColorShaderId);

		unsigned int whiteTexture;
		const char texData[] = { 255, 255, 255, 255 };
		//create 1 pixel white texture
		glGenTextures(1, &whiteTexture);
		glBindTexture(GL_TEXTURE_2D, whiteTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		ModelLibrary::GetInstance().m_whiteTextureID = whiteTexture;


		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		//skybox VAO, VBO
		glGenVertexArrays(1, &skyboxVAO);
		glBindVertexArray(skyboxVAO);

		glGenBuffers(1, &skyboxVBO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//std::vector<std::string> faces
		//{
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\right.jpg",
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\left.jpg",
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\top.jpg",
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\bottom.jpg",
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\front.jpg",
		//	_SOLUTIONDIR"Sandbox\\Assets\\skybox\\back.jpg"
		//};
		///*ModelLibrary::GetInstance().m_skyboxTextureID = */ModelLibrary::GetInstance().LoadCubemapTexture(faces);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::BeginFrame()
	{
#ifdef _DEBUG
		CheckGLError("Start of BeginFrame");
#endif // _DEBUG

		if (usedBuffers < frameBuffers.size())
		{
			frameBuffers.erase(frameBuffers.begin() + usedBuffers, frameBuffers.end());
		}

		usedBuffers = 0;


#ifdef _DEBUG
		CheckGLError("End of BeginFrame");
#endif // _DEBUG

	}

	void Renderer::PrepDraw(CameraComponent& camera)
	{

		glUseProgram(m_ShaderId);


		if (frameBuffers.size() <= usedBuffers)
		{
			BufferData newBuffer;
			glGenFramebuffers(1, &newBuffer.framebuffer);
			glGenTextures(1, &newBuffer.renderedTexture);
			glGenRenderbuffers(1, &newBuffer.renderbuffer);
			frameBuffers.push_back(newBuffer);
		}
		
		BufferData& data = camera.m_BufferData;

		if (frameBuffers[usedBuffers].bufferHeigth  != data.bufferHeigth || frameBuffers[usedBuffers].bufferWidth != data.bufferWidth)
		{
			frameBuffers[usedBuffers].bufferHeigth = data.bufferHeigth;
			frameBuffers[usedBuffers].bufferWidth = data.bufferWidth;
			Renderer::RecreateFramebuffer(frameBuffers[usedBuffers]);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[usedBuffers].framebuffer);
		camera.m_BufferData = frameBuffers[usedBuffers];


		glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, data.bufferHeigth, data.bufferWidth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		glUseProgram(m_ShaderId);
		//game window draw stuff
		glEnable(GL_CULL_FACE);
		//glDisable(GL_CULL_FACE);


		glm::mat4 projection = glm::perspective(camera.fov, (float)camera.m_BufferData.bufferHeigth / camera.m_BufferData.bufferWidth, 0.1f, camera.renderDistance);
		Renderer::SetShaderUniformMat4(m_ShaderId, "projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		Renderer::SetShaderUniformMat4(m_ShaderId, "view", view);

		glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);
		Renderer::SetShaderUniformVec3(m_ShaderId, "color", quadColor);

		glm::mat4 model = glm::mat4(1.0f);
		Renderer::SetShaderUniformMat4(m_ShaderId, "model", model);

		glUseProgram(m_TextureShaderId);

		Renderer::SetShaderUniformVec3(m_TextureShaderId, "viewPos", camera.GetPosition());

		Renderer::SetShaderUniformMat4(m_TextureShaderId, "projection", projection);
		Renderer::SetShaderUniformMat4(m_TextureShaderId, "view", view);


		glUseProgram(m_VertexColorShaderId);

		Renderer::SetShaderUniformVec3(m_VertexColorShaderId, "viewPos", camera.GetPosition());

		Renderer::SetShaderUniformMat4(m_VertexColorShaderId, "projection", projection);
		Renderer::SetShaderUniformMat4(m_VertexColorShaderId, "view", view);

		glUseProgram(m_SkyboxShaderId);

		Renderer::SetShaderUniformMat4(m_SkyboxShaderId, "projection", projection);
		Renderer::SetShaderUniformMat4(m_SkyboxShaderId, "view", glm::mat4(glm::mat3(view)));

		glUseProgram(0);
	}

	void Renderer::DrawMesh(MeshComponent mesh, TransformComponent transform)
	{
#ifdef _DEBUG
		CheckGLError("Start of DrawMesh");
#endif // _DEBUG

		unsigned int shaderID = ModelLibrary::GetInstance().GetShader(mesh.m_shaderID);
		glUseProgram(shaderID);

		glm::mat4 model = TransformSystem::GetModelMatrix(transform);

		Renderer::SetShaderUniformMat4(shaderID, "model", model);

		Mesh* m = ModelLibrary::GetInstance().GetMesh(mesh.m_meshID);
		Material& mat = ModelLibrary::GetInstance().GetMaterial(mesh.m_materialID);
		//m->Draw(shaderID);

		int count = 0;

		Renderer::SetShaderUniformVec3(shaderID, "material.diffuseColor", mat.diffuseColor);
		Renderer::SetShaderUniformVec3(shaderID, "specularColor", mat.specularColor);

		if (mat.diffuseTextures.empty())
		{
			glActiveTexture(GL_TEXTURE0);
			Renderer::SetShaderUniformInt(shaderID, "material.diffuse1", 0);
			glBindTexture(GL_TEXTURE_2D, ModelLibrary::GetInstance().GetWhiteTexture());
			count++;
			
		}
		else
		{

			unsigned int diffuseNr = 1;
			for (unsigned int i = 0; i < mat.diffuseTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				std::string number;
				std::string type = mat.diffuseTextures[i].type;
			
				assert(type == "diffuse");
				number = std::to_string(diffuseNr++);
				std::string uniformName = "material.";
				uniformName = uniformName + type + number;
				Renderer::SetShaderUniformInt(shaderID, uniformName.c_str(), i);
				glBindTexture(GL_TEXTURE_2D, mat.diffuseTextures[i].id);
				count++;
			}
		}

		if (mat.specularTextures.empty())
		{
			glActiveTexture(GL_TEXTURE0 + count);
			Renderer::SetShaderUniformInt(shaderID, "material.specular1", count);
			glBindTexture(GL_TEXTURE_2D, ModelLibrary::GetInstance().GetWhiteTexture());
			count++;
		}
		else
		{
			unsigned int specularNr = 1;
			for (unsigned int i = 0; i < mat.specularTextures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + count + i);
				std::string number;
				std::string type = mat.specularTextures[i].type;

				assert(type == "specular");
				number = std::to_string(specularNr++);

				std::string uniformName = "material.";
				uniformName = uniformName + type + number;
				Renderer::SetShaderUniformInt(shaderID, uniformName.c_str(), count + i);
				glBindTexture(GL_TEXTURE_2D, mat.specularTextures[i].id);
				count++;
			}
		}
		SetShaderUniformFloat(shaderID, "material.shininess", mat.shininess);

		glBindVertexArray(m->m_VAO);

		if (m->bIndiced)
			glDrawElements(GL_TRIANGLES, m->m_indices.size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m->m_vertices.size());

		glBindVertexArray(0);
		//unbinds textures

		for (unsigned int i = 0; i < count; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
#ifdef _DEBUG
		CheckGLError("End of DrawMesh");
#endif // _DEBUG
		glUseProgram(0);

	}

	/// <summary>
	/// Used when running without UI
	/// </summary>
	/// <param name="texture"></param>
	void Renderer::DrawSceneTextureOnScreen(int texture)
	{

		if (!screenQuadMesh)
		{
			Tex tex;
			tex.id = texture;
			tex.type = "diffuse";
			std::vector<Vertex> vertices;
			vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
			vertices.push_back(Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
			vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
			vertices.push_back(Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
			vertices.push_back(Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
			vertices.push_back(Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));
			std::vector<Tex> textureVec;
			textureVec.push_back(tex);
			screenQuadMesh = new Mesh(vertices, textureVec);
		}
		unsigned int shaderID = ModelLibrary::GetInstance().GetShader("quadShader");
		glUseProgram(shaderID);

		screenQuadMesh->Draw(shaderID);

		glUseProgram(0);
	}


	void Renderer::DrawCubemap()
	{

		unsigned int skyboxTex = ModelLibrary::GetInstance().m_skyboxTextureID;
		if (skyboxTex == 0)
			return;
		//Skybox
		glDepthFunc(GL_LEQUAL);
		glUseProgram(m_SkyboxShaderId);

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ModelLibrary::GetInstance().m_skyboxTextureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDepthMask(GL_TRUE);
		//glStencilMask(0xFF);
	}

	unsigned int Renderer::EndBufferRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		
		usedBuffers++;
		return frameBuffers[usedBuffers-1].renderedTexture;
		//Got some weird results without this :/
	}

	/*
	BufferData& Renderer::AddFramebuffer()
	{
		CheckGLError(" AddFrameBuffer");
		BufferData data;
		glGenFramebuffers(1, &data.framebuffer);
		glGenTextures(1, &data.renderedTexture);
		glGenRenderbuffers(1, &data.renderbuffer);

		frameBuffers.push_back(data);
		RecreateFramebuffer(data);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return data;
	}

	void Renderer::SetFramebuffer(int id)
	{
		boundBuffer = id;
	}
	*/

	void Renderer::RecreateFramebuffer(BufferData& data)
	{

		glBindFramebuffer(GL_FRAMEBUFFER, data.framebuffer);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, data.renderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, data.bufferHeigth, data.bufferWidth, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, data.renderedTexture, 0);
		
		glBindRenderbuffer(GL_RENDERBUFFER, data.renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, data.bufferHeigth, data.bufferWidth);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, data.renderbuffer);

		glBindTexture(GL_TEXTURE_2D, 0);


		GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		std::string errorstring;
		switch (framebufferStatus)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			return;
		case GL_FRAMEBUFFER_UNDEFINED:
			errorstring = "Undefined framebuffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			errorstring = "Incomplete Attachment!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			errorstring = "Imcomplete Missing Attachment!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			errorstring = "Incomplete Draw Buffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			errorstring = "Incomplete Read Buffer!";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			errorstring = "Unsupported Framebuffer!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			errorstring = "Incomplete Multisample!";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			errorstring = "Incomplete Layer Targets!";
			break;
		default:
			errorstring = "Unknown Error";
			break;
		}

		std::cout << "ERROR::FRAMEBUFFER: " << errorstring << std::endl;

	}

#ifdef _DEBUG
	void Renderer::CheckGLError(std::string placeMessage)
	{
		while (true)
		{
			const GLenum err = glGetError();
			if (GL_NO_ERROR == err)
				break;
			std::string errorstring;
			switch (err)
			{
			case GL_NO_ERROR:
				errorstring = "No error";
				break;
			case GL_INVALID_ENUM:
				errorstring = "Invalid enum";
				break;
			case GL_INVALID_VALUE:
				errorstring = "Invalid value";
				break;
			case GL_INVALID_OPERATION:
				errorstring = "Invalid operation";
				break;
			case GL_STACK_OVERFLOW:
				errorstring = "Stack overflow";
				break;
			case GL_STACK_UNDERFLOW:
				errorstring = "Stack underflow";
				break;
			case GL_OUT_OF_MEMORY:
				errorstring = "Out of memory";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorstring = "Invalid Framebuffer Operation";
				break;
			case GL_CONTEXT_LOST:
				errorstring = "Context Lost";
				break;
			default:
				errorstring = "Unknown Error";
				break;
			}

			std::cout << "GL Error: " << errorstring << " Placement: " << placeMessage << std::endl;
		}
	}
#endif // _DEBUG

	unsigned int Renderer::CreateShader(const char* vertShaderPath, const char* fragShaderPath)
	{

		std::ifstream v(vertShaderPath);
		std::string vertString;
		if (v)
		{
			std::ostringstream ss;
			ss << v.rdbuf();
			vertString = ss.str();
		}

		std::ifstream f(fragShaderPath);

		std::string fragString;
		if (f)
		{
			std::ostringstream ss;
			ss << f.rdbuf();
			fragString = ss.str();
		}

		const char* vert = vertString.c_str();
		const char* frag = fragString.c_str();
		int success;

		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vert, NULL);
		glCompileShader(vertShader);
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(vertShader, logLength, NULL, infoLog);
			std::cout << "VERTEX SHADER COMPILATION FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteShader(vertShader);
			return 0;
		}

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &frag, NULL);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "FRAGMENT SHADER COMPILATION FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteShader(fragShader);
			return 0;
		}

		unsigned int Id = glCreateProgram();
		glAttachShader(Id, vertShader);
		glAttachShader(Id, fragShader);
		glLinkProgram(Id);

		int isLinked;
		glGetProgramiv(Id, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			int logLength = 0;
			glGetProgramiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetProgramInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "SHADER LINKING FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteProgram(fragShader);
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			return 0;

		}
		glDetachShader(Id, vertShader);
		glDetachShader(Id, fragShader);

		return Id;
	}

	/// <summary>
	/// Untested
	/// </summary>
	/// <param name="vertShaderPath"></param>
	/// <param name="geometryShaderPath"></param>
	/// <param name="fragShaderPath"></param>
	/// <returns></returns>
	unsigned int Renderer::CreateShader(const char* vertShaderPath, const char* geometryShaderPath, const char* fragShaderPath)
	{
		std::ifstream v(vertShaderPath);
		std::string vertString;
		if (v)
		{
			std::ostringstream ss;
			ss << v.rdbuf();
			vertString = ss.str();
		}

		std::ifstream g(fragShaderPath);
		std::string geomString;
		if (g)
		{
			std::ostringstream ss;
			ss << g.rdbuf();
			geomString = ss.str();
		}

		std::ifstream f(fragShaderPath);
		std::string fragString;
		if (f)
		{
			std::ostringstream ss;
			ss << f.rdbuf();
			fragString = ss.str();
		}

		const char* vert = vertString.c_str();
		const char* geom = vertString.c_str();
		const char* frag = fragString.c_str();
		int success;

		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vert, NULL);
		glCompileShader(vertShader);
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{

			int logLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(vertShader, logLength, NULL, infoLog);
			std::cout << "VERTEX SHADER COMPILATION FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteShader(vertShader);
			return 0;
		}

		GLuint geomShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(geomShader, 1, &vert, NULL);
		glCompileShader(geomShader);
		glGetShaderiv(geomShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{

			int logLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(geomShader, logLength, NULL, infoLog);
			std::cout << "VERTEX SHADER COMPILATION FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteShader(geomShader);
			return 0;
		}


		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &frag, NULL);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int logLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "FRAGMENT SHADER COMPILATION FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteShader(fragShader);
			return 0;
		}

		unsigned int Id = glCreateProgram();
		glAttachShader(Id, vertShader);
		glAttachShader(Id, fragShader);
		glLinkProgram(Id);

		int isLinked;
		glGetProgramiv(Id, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			int logLength;
			glGetProgramiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
			char* infoLog = new char[logLength];
			glGetProgramInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "SHADER LINKING FAILED:" << infoLog << std::endl;
			delete[] infoLog;
			glDeleteProgram(fragShader);
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			return 0;

		}
		glDetachShader(Id, vertShader);
		glDetachShader(Id, fragShader);

		return Id;
	}

	void Renderer::SetPointLightValues(unsigned int shaderID, TransformComponent& transform, PointLightComponent& pointLight)
	{
		glUseProgram(shaderID);
		Renderer::SetShaderUniformVec3(shaderID, "pointLight.position", TransformSystem::GetPosition(transform));
		Renderer::SetShaderUniformVec3(shaderID, "pointLight.ambient", pointLight.m_ambient);
		Renderer::SetShaderUniformVec3(shaderID, "pointLight.diffuse", pointLight.m_diffuse);
		Renderer::SetShaderUniformVec3(shaderID, "pointLight.specular", pointLight.m_specular);
		Renderer::SetShaderUniformFloat(shaderID, "pointLight.constant", pointLight.m_constant);
		Renderer::SetShaderUniformFloat(shaderID, "pointLight.linear", pointLight.m_linear);
		Renderer::SetShaderUniformFloat(shaderID, "pointLight.quadratic", pointLight.m_quadratic);
		glUseProgram(0);
	}

	void Renderer::SetDirLightValues(unsigned int shaderID, TransformComponent& transform, DirLightComponent& dirLight)
	{
		glUseProgram(shaderID);
		Renderer::SetShaderUniformVec3(shaderID, "dirLight.direction", dirLight.m_direction);
		Renderer::SetShaderUniformVec3(shaderID, "dirLight.ambient", dirLight.m_ambient);
		Renderer::SetShaderUniformVec3(shaderID, "dirLight.diffuse", dirLight.m_diffuse);
		Renderer::SetShaderUniformVec3(shaderID, "dirLight.specular", dirLight.m_specular);
		glUseProgram(0);
	}

	void Renderer::SetShaderUniformFloat(unsigned int m_ShaderId, const char* name, float f)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniform1f(loc, f);
	}

	void Renderer::SetShaderUniformInt(unsigned int m_ShaderId, const char* name, int i)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniform1i(loc, i);
	}

	void Renderer::SetShaderUniformBool(unsigned int m_ShaderId, const char* name, bool b)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniform1i(loc, b);
	}

	void Renderer::SetShaderUniformVec2(unsigned int m_ShaderId, const char* name, glm::vec2 vector)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniform2f(loc, vector.x, vector.y);
	}

	void Renderer::SetShaderUniformVec3(unsigned int m_ShaderId, const char* name, glm::vec3 vector)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniform3f(loc, vector.x, vector.y, vector.z);
	}

	void Renderer::SetShaderUniformMat4(unsigned int m_ShaderId, const char* name, glm::mat4 matrix)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	/*void Renderer::SetShaderUniformMat4(const char* name, glm::mat4 matrix)
	{
		unsigned int loc = glGetUniformLocation(m_ShaderId, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}*/
}