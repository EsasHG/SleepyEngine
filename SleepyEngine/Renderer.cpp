#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void Renderer::SetShaderUniformFloat(unsigned int shaderId, const char* name, float f)
{
	unsigned int loc = glGetUniformLocation(shaderId, name);
	glUniform1f(loc, f);
}

void Renderer::SetShaderUniformInt(unsigned int shaderId, const char* name, int i)
{
	unsigned int loc = glGetUniformLocation(shaderId, name);
	glUniform1i(loc, i);
}

void Renderer::SetShaderUniformVec2(unsigned int shaderId, const char* name, glm::vec2 vector)
{
	unsigned int loc = glGetUniformLocation(shaderId, name);
	glUniform2f(loc, vector.x, vector.y);
}

void Renderer::SetShaderUniformVec3(unsigned int shaderId, const char* name, glm::vec3 vector)
{
	unsigned int loc = glGetUniformLocation(shaderId, name);
	glUniform3f(loc, vector.x, vector.y, vector.z);
}

void Renderer::SetShaderUniformMat4(unsigned int shaderId, const char* name, glm::mat4 matrix)
{
	unsigned int loc = glGetUniformLocation(shaderId, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));

}
