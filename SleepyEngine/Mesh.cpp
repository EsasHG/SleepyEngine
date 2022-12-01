#include "Mesh.h"
#include <glad/glad.h>
#include "Renderer.h"
Mesh::Mesh(std::vector<Vertex> vertices) : m_vertices(vertices)
{
	bIndiced = false;
	SetupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Tex> textures) : m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	bIndiced = true;
	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_uv));
	glEnableVertexAttribArray(2);

	if (bIndiced)
	{
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
}

//TODO: should the renderer be responsible for this?
void Mesh::Draw(unsigned int shaderID)
{
	if (!m_textures.empty())
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string type = m_textures[i].type;
			if (type == "diffuse")
				number = std::to_string(diffuseNr++);
			else if (type == "specular")
				number = std::to_string(specularNr++);
			Renderer::SetShaderUniformInt(shaderID, (type + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}
	}

	glBindVertexArray(m_VAO);

	if (bIndiced)
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
	//unbinds textures
	if (!m_textures.empty())
	{
		for (unsigned int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
	}

}
