#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>


namespace Sleepy
{
	struct MeshGroup
	{
		struct MeshRef* firstChild = nullptr;
	
		MeshGroup* parentGroup = nullptr;
		MeshGroup* firstChildGroup = nullptr;
	
		MeshGroup* prevGroup = nullptr;
		MeshGroup* nextGroup = nullptr;
	};
	
	struct MeshRef 
	{
		std::string meshName;
		std::string materialName;
		MeshGroup* parentGroup = nullptr;
		MeshRef* prevChildMesh = nullptr;
		MeshRef* nextChildMesh = nullptr;
	};
	
	struct Vertex
	{
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) : m_position(position), m_normal(normal), m_uv(uv)
		{};
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_uv;
	};
	
	//TODO Make namespace and then rename to Texture. ie SLEEPY::TEXTURE orsmth
	struct Tex
	{
		unsigned int id;
		std::string type;
		std::string path;
	
		bool operator == (const Tex& a)
		{
			return id == a.id;
		}
	};
	
	struct Material
	{
		glm::vec3 ambientColor = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
		std::vector<Tex> diffuseTextures;
		glm::vec3 specularColor = glm::vec3(0.5f,0.5f,0.5f);
		std::vector<Tex> specularTextures;
		float shininess = 32.0f;
	};
	
	class Mesh
	{
	public:
	
		Mesh(std::vector<Vertex> vertices);
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Tex> textures);
		Mesh(std::vector<Vertex> vertices, std::vector<Tex> textures);
		void Draw(unsigned int shaderID);
	private:
	
		void SetupMesh();
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		//TODO: move away from mesh?
		std::vector<Tex> m_textures;
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;
		bool bIndiced;
	
		friend class ModelLibrary;
		friend class Renderer;
	};

}