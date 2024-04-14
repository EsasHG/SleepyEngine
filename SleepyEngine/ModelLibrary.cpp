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

namespace Sleepy
{
	ModelLibrary::ModelLibrary()
	{

	}

	void ModelLibrary::SetupDefaultMeshes()
	{
		std::vector<Sleepy::Vertex> vertices;
		vertices.reserve(sizeof(Sleepy::Vertex) * 6);
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)));

		Sleepy::ModelLibrary::GetInstance().AddMesh("quad", vertices);

		vertices.clear();

		vertices.reserve(sizeof(Sleepy::Vertex) * 6 * 6);
		// positions					// normals									// texture coords
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)));

		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));

		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));

		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
		vertices.push_back(Sleepy::Vertex(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));

		Sleepy::ModelLibrary::GetInstance().AddMesh("Cube", vertices);

	}

	
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns>Names of all the meshes and submeshes loaded in</returns>
	MeshGroup* ModelLibrary::AddMesh(std::string filepath)
	{
		if (ModelExists(filepath))
			return m_models[filepath];
		else
			return LoadModel(filepath);
	}
	
	bool ModelLibrary::AddMesh(std::string name, std::vector<Vertex> vertices)
	{
		if (MeshExists(name))
			return false;
		Mesh* m = new Mesh(vertices);
		std::pair<std::string, Mesh*> pair(name, m);
		m_meshes.insert(pair);
		return true;
	}
	
	/// <summary>
	/// Tries to load texture if texture.path is set.
	/// This probably doesn't work properly.
	/// </summary>
	/// <param name="name"></param>
	/// <param name="vertices"></param>
	/// <param name="texture"></param>
	/// <returns></returns>
	bool ModelLibrary::AddMesh(std::string name, std::vector<Vertex> vertices, Tex texture)
	{
		if (MeshExists(name))
			return false;
		std::vector<Tex> textures;
		textures.push_back(texture);
		Mesh* m = new Mesh(vertices, textures);
		std::pair<std::string, Mesh*> pair(name, m);
		m_meshes.insert(pair);
		if (texture.path != "")
		{
			auto it = std::find(m_loadedTextures.begin(), m_loadedTextures.end(), texture);
			if (it == m_loadedTextures.end())
			{
				texture.path.substr(0, texture.path.find_last_of("\\"));
				std::string directory = texture.path.substr(0, texture.path.find_last_of("\\"));
				std::string name = texture.path.substr(texture.path.find_last_of("\\") + 1, texture.path.length());	//remove path
				LoadTexture(name.c_str(), directory);
				m_loadedTextures.push_back(texture);
			}
		}
		m->m_textures.push_back(texture);
		return true;
	}
	
	bool ModelLibrary::AddMesh(std::string name, Mesh* mesh)
	{
	
		if (MeshExists(name))
			return false;
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
	
	void ModelLibrary::AddMaterial(std::string name, Material mat)
	{
		m_materials.insert(std::pair<std::string, Material>(name, mat));
	}
	
	
	//std::vector<std::string> ModelLibrary::LoadModel(std::string path)
	//{
	//	std::vector<std::string> meshNames;
	//	Assimp::Importer importer;
	//
	//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//
	//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	{
	//		std::cout << "ERROR::ASSIMP:" << importer.GetErrorString() << std::endl;
	//		return meshNames;
	//	}
	//	std::string directory = path.substr(0, path.find_last_of("/"));
	//	std::string meshName = path.substr(path.find_last_of("/")+1, path.length());
	//	meshNames = ProcessModelNode(scene->mRootNode, scene, meshName, directory);
	//	return meshNames;
	//}
	
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
	
	MeshGroup* ModelLibrary::LoadModel(std::string path)
	{
		MeshGroup* rootGroup = new MeshGroup;
		Assimp::Importer importer;
	
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:" << importer.GetErrorString() << std::endl;
			return nullptr;
		}
		std::string directory = path.substr(0, path.find_last_of("\\"));
		std::string meshName = path.substr(path.find_last_of("\\") + 1, path.length());	//remove path
		meshName = meshName.substr(0, meshName.find_last_of("."));						//remove extention
		
		//adds all meshes into library beforehand, and use the name to get them later. Otherwise they might be added more than once
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			ProcessMesh(scene->mMeshes[i], scene, directory, meshName + "_" + std::to_string(i));
		}
	
		MeshGroup* group = ProcessModelNode(scene->mRootNode, scene, meshName, directory, rootGroup);
	
		if (group == rootGroup)
		{
			if (group->firstChildGroup)
			{
	
				std::pair<std::string, MeshGroup*> pair(path, group);
				m_models.insert(pair);
				return group;
			}
		}
		else
		{
			rootGroup->firstChildGroup = group;
			std::pair<std::string, MeshGroup*> pair(path, rootGroup);
			m_models.insert(pair);
			return rootGroup;
		}
			
		return nullptr;
	}
	
	MeshGroup* ModelLibrary::ProcessModelNode(aiNode* node, const aiScene* scene, std::string meshName, std::string directory, MeshGroup* targetGroup)
	{
		MeshGroup* group;
		//meshes 
		if (node->mNumMeshes > 0)
		{
			group = new MeshGroup();
			group->parentGroup = targetGroup;
	
			MeshRef* currentChildMesh = nullptr;
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				std::string currentMeshName = meshName +"_"+ std::to_string(node->mMeshes[i]);
	
				aiString matName;
				scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]->Get(AI_MATKEY_NAME, matName);
	
				MeshRef* mRef = new MeshRef();
				mRef->meshName = currentMeshName;
				mRef->materialName = matName.C_Str();
	
				if (!group->firstChild)
				{
					group->firstChild = mRef;
				}
				else if (currentChildMesh)
				{
					mRef->prevChildMesh = currentChildMesh;
					currentChildMesh->nextChildMesh = mRef;
				}
				else
				{
					assert("This should not be possible!");
				}
				currentChildMesh = mRef;
				currentChildMesh->parentGroup = group;
			}
		}
		else
		{
			group = targetGroup;
		}
	
		//child nodes
		MeshGroup* currentChildGroup = nullptr;
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			MeshGroup* childGroup = ProcessModelNode(node->mChildren[i], scene, meshName, directory, group);
	
			if (childGroup != group) //might happen if children have no meshes?
			{
				if (!group->firstChildGroup)
				{
					group->firstChildGroup = childGroup;
				}
				else if (currentChildGroup)
				{
					childGroup->prevGroup = currentChildGroup;
					currentChildGroup->nextGroup = childGroup;
				}
	
				currentChildGroup = childGroup;
			}
		}
		return group;
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
			if (mesh->mTextureCoords[0]) // mesh->HasTextureCoords()?
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
			aiString matName;
			material->Get(AI_MATKEY_NAME, matName);
			if (!m_materials.contains(matName.C_Str()))
			{
				Material mat;
				float shininess = 0.0f;
				
				if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess))
					mat.shininess = 32.0f;
				else
					mat.shininess = shininess;
	
				mat.diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse", directory);
				textures.insert(textures.end(), mat.diffuseTextures.begin(), mat.diffuseTextures.end());
	
				mat.specularTextures = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular", directory);
				textures.insert(textures.end(), mat.specularTextures.begin(), mat.specularTextures.end());
	
				AddMaterial(matName.C_Str(), mat);
			}
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
		fileName = dir + "\\" + fileName;
	
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
			glBindTexture(GL_TEXTURE_2D, 0);
	
		}
		else
		{
			std::cout << "Failed to load texture at path " << dir << path << std::endl;
		}
	
		stbi_image_free(data);
	
		return textureID;
	}

	unsigned int ModelLibrary::LoadCubemapTexture(std::vector<std::string>& faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			stbi_set_flip_vertically_on_load(false);
			
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				GLenum format;
				switch (nrChannels) {
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
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path " << faces[i] << std::endl;
				stbi_image_free(data);
				return 0;

			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_skyboxTextureID = textureID;
		return textureID;
	}


	unsigned int ModelLibrary::LoadCubemapTexture(std::string& faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(false);
		stbi_uc;
		unsigned char* data = stbi_load(faces.c_str(), &width, &height, &nrChannels, 0);

		int bytesPerPixel = sizeof(data) * width * height;

		if (data)
		{
			GLenum format;
			switch (nrChannels) {
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


			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * width *		(height / 3)));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * width *		(height / 3)));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * width *		(height / 3)));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * (width * 2) * (height / 3)));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * width *		(height / 3)));
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, data + (bytesPerPixel * width *		(height / 3)));
			m_skyboxTextureID = textureID;

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path " << faces << std::endl;
			stbi_image_free(data);
			return 0;
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;

	}

}