#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
class Model
{
public:
	//TODO remove shaderID from constructors
	Model(std::string path);
	void Draw(unsigned int shaderID);
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Tex> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int LoadTexture(const char* path, std::string dir);
	std::string directory;

	std::vector<Mesh> m_meshes;
	std::vector<Tex> m_loadedTextures;

	

};

