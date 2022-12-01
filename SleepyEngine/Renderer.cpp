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
#include "TransformSystem.h"
//should this be here? 
#include "Camera.h"


Renderer::Renderer(glm::vec2 windowSize) : m_WindowSize(windowSize)
{
	if (!gladLoadGL())
	{
		std::cout << "Glad did not load successfully!" << std::endl;
	}

	m_ShaderId = Renderer::CreateShader("Shaders/SingleColor.vert", "Shaders/SingleColor.frag");
	ModelLibrary::GetInstance().AddShader("color", m_ShaderId);
	m_TextureShaderId = Renderer::CreateShader("Shaders/TextureShader.vert", "Shaders/TextureShader.frag");
	ModelLibrary::GetInstance().AddShader("default", m_TextureShaderId);
	 
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &renderedTexture);
	glGenRenderbuffers(1, &RBO);

	RecreateFramebuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer()
{
}

void Renderer::BeginFrame(glm::vec2 windowSize)
{
	CheckGLError("Start of BeginFrame");

	glUseProgram(m_ShaderId);

	//if 0 we might get a divide by 0 error below. This should really be handled some other way
	//was not like this earlier. No idea why it started happening
	if (windowSize.y != 0)
		m_WindowSize = windowSize;
	else
		m_WindowSize = glm::vec2(1920, 1080);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glm::vec3 clearColor = glm::vec3(0.7f, 0.3f, 0.6f);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CheckGLError("End of BeginFrame");
}
void Renderer::SetCamera(class Camera* camera) 
{ 
	m_camera = camera;
}

//TODO: Sets all values that only needs to be set once per shader per frame. Should be done in another way?
unsigned int Renderer::Draw(double deltaTime)
{
	glUseProgram(m_ShaderId);
	//game window draw stuff
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	glm::mat4 projection = glm::perspective(0.5f, (float)m_WindowSize.x / m_WindowSize.y, 0.1f, 100.0f);
	Renderer::SetShaderUniformMat4(m_ShaderId, "projection", projection);
	glm::mat4 view = m_camera->GetViewMatrix();
	Renderer::SetShaderUniformMat4(m_ShaderId, "view", view);

	glm::vec3 quadColor = glm::vec3(0.4f, 0.4f, 0.4f);
	Renderer::SetShaderUniformVec3(m_ShaderId, "color", quadColor);

	glm::mat4 model = glm::mat4(1.0f);
	Renderer::SetShaderUniformMat4(m_ShaderId, "model", model);

	glUseProgram(m_TextureShaderId);

	Renderer::SetShaderUniformVec3(m_TextureShaderId, "viewPos", m_camera->GetPosition());

	Renderer::SetShaderUniformMat4(m_TextureShaderId, "projection", projection);
	Renderer::SetShaderUniformMat4(m_TextureShaderId, "view", view);

	glDisable(GL_DEPTH_TEST);
	
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return renderedTexture;
}

void Renderer::DrawMesh(MeshComponent mesh, TransformComponent transform)
{
	CheckGLError("Start of DrawMesh");

	unsigned int shaderID = ModelLibrary::GetInstance().GetShader(mesh.m_shaderID);
	glUseProgram(shaderID);

	glm::mat4 model = TransformSystem::GetModelMatrix(&transform);
	//model = glm::translate(model, transform.position);
	////model = glm::rotate(model, glm::radians(90.0f), transform.rotation);
	//model = glm::scale(model, transform.scale);

	Renderer::SetShaderUniformMat4(shaderID, "model", model);

	Mesh* m = ModelLibrary::GetInstance().GetMesh(mesh.m_meshID);
	m->Draw(shaderID);
	CheckGLError("End of DrawMesh");
	glUseProgram(0);

}

void Renderer::SetPointLightValues(unsigned int shaderID, TransformComponent& transform, PointLightComponent& pointLight)
{
	glUseProgram(shaderID);
	Renderer::SetShaderUniformVec3(shaderID, "pointLight.position", TransformSystem::GetGlobalPosition(&transform));
	Renderer::SetShaderUniformVec3(shaderID, "pointLight.ambient", pointLight.m_ambient);
	Renderer::SetShaderUniformVec3(shaderID, "pointLight.diffuse", pointLight.m_diffuse);
	Renderer::SetShaderUniformVec3(shaderID, "pointLight.specular", pointLight.m_specular);
	Renderer::SetShaderUniformFloat(shaderID, "pointLight.constant", pointLight.m_constant);
	Renderer::SetShaderUniformFloat(shaderID, "pointLight.linear", pointLight.m_linear);
	Renderer::SetShaderUniformFloat(shaderID, "pointLight.quadratic",pointLight.m_quadratic);
}

void Renderer::SetDirLightValues(unsigned int shaderID, TransformComponent& transform, DirLightComponent& dirLight)
{
	glUseProgram(shaderID);
	Renderer::SetShaderUniformVec3(shaderID, "dirLight.direction", dirLight.m_direction);
	Renderer::SetShaderUniformVec3(shaderID, "dirLight.ambient", dirLight.m_ambient);
	Renderer::SetShaderUniformVec3(shaderID, "dirLight.diffuse", dirLight.m_diffuse);
	Renderer::SetShaderUniformVec3(shaderID, "dirLight.specular", dirLight.m_specular);
}

void Renderer::EndFrame()
{
	//Got some weird results without this :/
	RecreateFramebuffer();
}

void Renderer::ResizeViewport(int x, int y)
{

	//We don't set window height/width here? We set it each frame so we might not have to... 

	m_WindowSize.x = x;
	m_WindowSize.y = y;

	glViewport(0, 0, x, y);
	RecreateFramebuffer();
}


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

void Renderer::SetShaderUniformFloat(unsigned int m_SshaderId, const char* name, float f)
{
	unsigned int loc = glGetUniformLocation(m_SshaderId, name);
	glUniform1f(loc, f);
}

void Renderer::SetShaderUniformInt(unsigned int m_SshaderId, const char* name, int i)
{
	unsigned int loc = glGetUniformLocation(m_SshaderId, name);
	glUniform1i(loc, i);
}

void Renderer::SetShaderUniformVec2(unsigned int m_SshaderId, const char* name, glm::vec2 vector)
{
	unsigned int loc = glGetUniformLocation(m_SshaderId, name);
	glUniform2f(loc, vector.x, vector.y);
}

void Renderer::SetShaderUniformVec3(unsigned int m_SshaderId, const char* name, glm::vec3 vector)
{
	unsigned int loc = glGetUniformLocation(m_SshaderId, name);
	glUniform3f(loc, vector.x, vector.y, vector.z);
}

void Renderer::SetShaderUniformMat4(unsigned int m_SshaderId, const char* name, glm::mat4 matrix)
{
	unsigned int loc = glGetUniformLocation(m_SshaderId, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Renderer::SetShaderUniformMat4(const char* name, glm::mat4 matrix)
{
	unsigned int loc = glGetUniformLocation(m_ShaderId, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Renderer::RecreateFramebuffer()
{

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowSize.x, m_WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_WindowSize.x, m_WindowSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

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