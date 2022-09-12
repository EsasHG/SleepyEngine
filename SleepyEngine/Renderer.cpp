#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

#include "InputManager.h"
#include "UiLayer.h"


Renderer::Renderer(glm::vec2 windowSize) : m_WindowSize(windowSize)
{
	if (!gladLoadGL())
	{
		std::cout << "Glad did not load successfully!" << std::endl;
	}

	InputManager::GetInstance().AddWindowResizeCallback(std::bind(&Renderer::FramebufferResizeCallback, this, std::placeholders::_1, std::placeholders::_2));
	ui = new UiLayer();

	m_ShaderId = Renderer::CreateShader("Shaders/SingleColor.vert", "Shaders/SingleColor.frag");
	m_QuadShaderId = Renderer::CreateShader("Shaders/QuadShader.vert", "Shaders/QuadShader.frag");
	glUseProgram(m_QuadShaderId);
	Renderer::SetShaderUniformInt(m_QuadShaderId, "sceneTexture", 0);
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowSize.x, m_WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Incomplete framebuffer!" << std::endl;

	glUseProgram(m_ShaderId);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Draw()
{
	m_WindowSize = ui->contentRegionSize;
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(ui->clearColor.x, ui->clearColor.y, ui->clearColor.z, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	//glEnable(GL_DEPTH_TEST);

	glUseProgram(m_ShaderId);
	glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_CULL_FACE);


	glm::mat4 projection = glm::perspective(0.5f, (float)m_WindowSize.x / m_WindowSize.y, 0.1f, 100.0f);
	Renderer::SetShaderUniformMat4(m_ShaderId, "projection", projection);

	Renderer::SetShaderUniformVec3(m_ShaderId, "color", ui->quadColor);

	glm::mat4 model = glm::mat4(1.0f);
	Renderer::SetShaderUniformMat4(m_ShaderId, "model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDisable(GL_DEPTH_TEST);
	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(m_QuadShaderId);
	glClear(GL_COLOR_BUFFER_BIT);

	ui->sceneTexture = renderedTexture;
	ui->Run();
	RecreateFramebuffer();
	//glBindVertexArray(VAO);
	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE, renderedTexture);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
//
	glUseProgram(m_ShaderId);

}

void Renderer::FramebufferResizeCallback(int x, int y)
{
	glViewport(0, 0, ui->contentRegionSize.x, ui->contentRegionSize.y);
	RecreateFramebuffer();
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

	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_WindowSize.x, m_WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Incomplete framebuffer!" << std::endl;
}