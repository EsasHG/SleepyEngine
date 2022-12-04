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

	bool ModelExists(std::string path)
	{
		return m_meshes.contains(path);
	}

	//std::vector<std::string> AddMesh(std::string filepath);
	MeshGroup* AddMesh(std::string filepath);

	bool AddMesh(std::string name, std::vector<Vertex> vertices);
	bool AddMesh(std::string name, std::vector<Vertex> vertices, Tex texture); //TODO: Finish 

	bool AddMesh(std::string name, Mesh* mesh);

	unsigned int GetShader(std::string name)
	{
		return m_shaders[name];
	}

	void AddShader(std::string name, std::string vertShaderPath, std::string fragShaderPath, std::string geomShaderPath = "");

	void AddShader(std::string name, unsigned int ShaderID);

	std::vector<std::string> GetMeshList()
	{
		std::vector<std::string> v; 

		for (auto pair : m_meshes)
			v.push_back(pair.first);

		return v;
	}

	std::vector<std::string> GetMaterialList()
	{
		std::vector<std::string> v;

		//for (auto pair : m_materials)
		//	v.push_back(pair.first);

		return v;
	}


	std::vector<std::string> GetShaderList()
	{
		std::vector<std::string> v;

		for (auto pair : m_shaders)
			v.push_back(pair.first);

		return v;
	}

private:
	ModelLibrary();

	//std::vector<std::string> LoadModel(std::string path);
	MeshGroup* LoadModel(std::string path);


	std::vector<std::string> ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory);
	//Mesh* ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory, Mesh* CurrentMesh);
	MeshGroup* ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory, MeshGroup* currentGroup);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string directory, std::string currentMeshName);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::string directory);
	unsigned int LoadTexture(const char* path, std::string dir);

	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, MeshGroup*> m_models;
	std::unordered_map<std::string, unsigned int> m_shaders;
	std::vector<Tex> m_loadedTextures;


};
