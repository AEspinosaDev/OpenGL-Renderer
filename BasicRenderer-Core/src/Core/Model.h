#pragma once
#include "Mesh.h"
#include "SceneObject.h"
#include "Materials/Material.h"



class Model : public SceneObject {
private:

	Mesh* m_Mesh;
	Material** m_Materials;

public:

	Model(const std::string na, Mesh* mesh, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(mesh), m_Materials(nullptr)
	{
		m_Materials = new Material * [m_Mesh->getNumberOfMeshes()];
		for (size_t i = 0; i < m_Mesh->getNumberOfMeshes(); i++)
		{
			m_Materials[i] = material;
		}
	}

	Model(const std::string na, const std::string meshFileRoute, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Materials(nullptr)
	{
		m_Materials = new Material * [m_Mesh->getNumberOfMeshes()];
		for (size_t i = 0; i < m_Mesh->getNumberOfMeshes(); i++)
		{
			m_Materials[i] = material;
		}
	}

	Model(Mesh* mesh, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(mesh), m_Materials(nullptr)
	{
		m_Materials = new Material * [m_Mesh->getNumberOfMeshes()];
		for (size_t i = 0; i < m_Mesh->getNumberOfMeshes(); i++)
		{
			m_Materials[i] = material;
		}
	}

	Model(const std::string meshFileRoute, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Materials(nullptr) 
	{
		m_Materials = new Material * [m_Mesh->getNumberOfMeshes()];
		for (size_t i = 0; i < m_Mesh->getNumberOfMeshes(); i++)
		{
			m_Materials[i] = material;
		}
	}

	~Model() { 
		delete m_Mesh;
		delete[] m_Materials;
	}

	void setMesh(const std::string& pname);

	void setMaterial(Material* m, unsigned int id = 0);

	inline Material* getMaterialReference(unsigned int id = 0) { return m_Materials[id]; }
	inline Mesh* getMesh() { return m_Mesh; }

	void draw(glm::mat4 proj, glm::mat4 view);

	Model* clone();

};