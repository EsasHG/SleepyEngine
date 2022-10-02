#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"

static class ModelLibrary* s_ModelLibrary;

class ModelLibrary
{

public:
	static ModelLibrary& GetInstance() {
		if (!s_ModelLibrary)
			s_ModelLibrary = new ModelLibrary();
		return *s_ModelLibrary;
	}

	Mesh* GetMesh(std::string name)
	{
		return m_meshes[name];
	}

	bool MeshExists(std::string name)
	{
		return m_meshes.contains(name);
	}
	bool AddMesh(std::string filepath);

	bool AddMesh(std::string name, std::vector<Vertex> vertices);

	unsigned int GetShader(std::string name)
	{
		return m_shaders[name];
	}

	void AddShader(std::string name, std::string vertShaderPath, std::string fragShaderPath, std::string geomShaderPath = "");

	void AddShader(std::string name, unsigned int ShaderID);
private:
	ModelLibrary();

	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, unsigned int> m_shaders;


};
