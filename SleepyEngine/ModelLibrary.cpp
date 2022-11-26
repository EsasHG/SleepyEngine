#include "ModelLibrary.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Components/MeshComponent.h"
#include "Model.h"

ModelLibrary::ModelLibrary()
{

}

/// <summary>
/// 
/// </summary>
/// <param name="filepath"></param>
/// <returns>Names of all the meshes and submeshes loaded in</returns>
std::vector<std::string> ModelLibrary::AddMesh(std::string filepath)
{
	return LoadModel(filepath);
}

bool ModelLibrary::AddMesh(std::string name, std::vector<Vertex> vertices)
{
	Mesh* m = new Mesh(vertices);
	std::pair<std::string, Mesh*> pair(name, m);
	m_meshes.insert(pair);
	return true;
}

bool ModelLibrary::AddMesh(std::string name, Mesh* mesh)
{
	std::pair<std::string, Mesh*> pair(name, mesh);
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


std::vector<std::string> ModelLibrary::LoadModel(std::string path)
{
	std::vector<std::string> meshNames;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:" << importer.GetErrorString() << std::endl;
		return meshNames;
	}
	std::string directory = path.substr(0, path.find_last_of("/"));
	std::string meshName = path.substr(path.find_last_of("/")+1, path.length());
	meshNames = ProcessModelNode(scene->mRootNode, scene, meshName, directory);
	return meshNames;
}

std::vector<std::string> ModelLibrary::ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory)
{
	std::vector<std::string> meshNames;
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		std::string currentMeshName = meshName + std::to_string(i);
		Mesh* m = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, directory,currentMeshName);
		meshNames.push_back(currentMeshName);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		std::vector<std::string> childMeshNames = ProcessModelNode(node->mChildren[i], scene, meshName + std::to_string(i), directory);
		meshNames.insert(meshNames.end(), childMeshNames.begin(), childMeshNames.end());
	}
	return meshNames;
}


Mesh* ModelLibrary::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string directory, std::string currentMeshName)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Tex> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec2 uv;
		glm::vec3 norm;

		glm::vec3 pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z);
		if (mesh->HasNormals())
		{
			norm = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);
		}
		else
		{
			norm = glm::vec3(0.0f);
		}
		if (mesh->mTextureCoords[0])
		{
			uv = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y);
		}
		else
		{
			uv = glm::vec2(0.0f);
		}
		vertices.push_back(Vertex(pos, norm, uv));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Tex> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse", directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Tex> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular", directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	//CreateEntity, addmeshcomponent, add transform, set parent
	Mesh* m = new Mesh(vertices, indices, textures);
	ModelLibrary::GetInstance().AddMesh(currentMeshName, m);

	return m;
}


std::vector<Tex> ModelLibrary::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::string directory)
{
	std::vector<Tex> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		//checks if the texture is already loaded
		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				//if already loaded use that one
				textures.push_back(m_loadedTextures[j]);
				skip = true;

				break;
			}
		}
		if (!skip)
		{
			Tex texture;
			texture.id = LoadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}
	return textures;
}


//TODO move this to some other place so we can load textures anywhere. A helper class maybe?
//Then we can remove the glad include from here as well
unsigned int ModelLibrary::LoadTexture(const char* path, std::string dir)
{

	std::string fileName = std::string(path);
	fileName = dir + "/" + fileName;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		switch (nrComponents) {
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = GL_RGBA;
			std::cout << "Model::LoadTexture: Unknown texture format!" << std::endl;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Failed to load texture at path " << dir << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}
