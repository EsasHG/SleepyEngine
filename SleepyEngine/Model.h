#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

struct aiNode;
struct aiMesh;
struct aiScene;
class Model
{
public:
	Model(std::string path);
	void Draw();
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::string directory;

	std::vector<Mesh> m_meshes;
};

