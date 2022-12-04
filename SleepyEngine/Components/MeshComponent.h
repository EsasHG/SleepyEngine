#pragma once
#include "Component.h"
#include <string>

#include "Mesh.h"
#include "Renderer.h"
#include "ModelLibrary.h"


class MeshComponent : public Component
{
public:
	MeshComponent(Entity* entity, std::string mesh, std::string material = "default", std::string shader = "default")
	{
		m_Entity = entity;
		if(ModelLibrary::GetInstance().MeshExists(mesh))
		{
			m_meshID = mesh;
		}
		else
		{
			ModelLibrary::GetInstance().AddMesh(mesh);
		}

		m_shaderID = shader;
		m_materialID = material;
	}

	std::string m_shaderID;
	std::string m_meshID;
	std::string m_materialID;
};