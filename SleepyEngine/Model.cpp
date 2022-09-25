#include "Model.h"

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Model::Model(std::string path, unsigned int shaderID) : m_ShaderID(shaderID)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));

	ProcessNode(scene->mRootNode, scene);
}

void Model::Draw()
{
	for (Mesh m : m_meshes)
	{
		m.Draw();
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		m_meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Tex> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Tex> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	
	return Mesh(m_ShaderID, vertices, indices, textures);
}

std::vector<Tex> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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
unsigned int Model::LoadTexture(const char* path, std::string dir)
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
