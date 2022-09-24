#pragma once
#include <glm/glm.hpp>
#include <vector>
struct Vertex
{
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) : m_position(position), m_normal(normal), m_uv(uv)
	{};
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_uv;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices);
	void Draw();
private:

	void SetupMesh();
	std::vector<Vertex> m_vertices;
	unsigned int m_VAO;
	unsigned int m_VBO;
};

