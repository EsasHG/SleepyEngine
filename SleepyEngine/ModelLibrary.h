#pragma once
#include <unordered_map>
#include <string>
#include "Mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

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
	std::vector<std::string> AddMesh(std::string filepath);

	bool AddMesh(std::string name, std::vector<Vertex> vertices);

	bool AddMesh(std::string name, Mesh* mesh);

	unsigned int GetShader(std::string name)
	{
		return m_shaders[name];
	}

	void AddShader(std::string name, std::string vertShaderPath, std::string fragShaderPath, std::string geomShaderPath = "");

	void AddShader(std::string name, unsigned int ShaderID);

private:
	ModelLibrary();

	std::vector<std::string> LoadModel(std::string path);
	std::vector<std::string> ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string directory, std::string currentMeshName);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::string directory);
	unsigned int LoadTexture(const char* path, std::string dir);

	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, unsigned int> m_shaders;
	std::vector<Tex> m_loadedTextures;


};
