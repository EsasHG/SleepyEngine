#include "ModelLibrary.h"
#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"

ModelLibrary::ModelLibrary()
{

}

bool ModelLibrary::AddMesh(std::string filepath)
{
	//Model m(filepath);
	return true;
}

bool ModelLibrary::AddMesh(std::string name, std::vector<Vertex> vertices)
{
	Mesh* m = new Mesh(vertices);
	std::pair<std::string, Mesh*> pair(name, m);
	m_meshes.insert(pair);
	return true;
}

void ModelLibrary::AddShader(std::string name, std::string vertShaderPath, std::string fragShaderPath, std::string geomShaderPath)
{
	if (geomShaderPath.empty())
	{
		unsigned int shaderID = Renderer::CreateShader(vertShaderPath.c_str(), fragShaderPath.c_str());
		m_shaders.insert(std::pair<std::string, unsigned int>(name, shaderID));
	}
	else
	{
		unsigned int shaderID = Renderer::CreateShader(vertShaderPath.c_str(), geomShaderPath.c_str(), fragShaderPath.c_str());
		m_shaders.insert(std::pair<std::string, unsigned int>(name, shaderID));
	}
}

void ModelLibrary::AddShader(std::string name, unsigned int ShaderID)
{
	m_shaders.insert(std::pair<std::string, unsigned int>(name, ShaderID));
}


