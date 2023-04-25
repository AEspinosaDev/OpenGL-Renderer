#pragma once
#include "Mesh.h"
#include "SceneObject.h"
#include "Materials/Material.h"



class Model : public SceneObject {
private:

	Mesh* m_Mesh;
	Material* m_Mat;

public:

	Model(const std::string na, Mesh* mesh, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(mesh), m_Mat(material) {}

	Model(const std::string na, const std::string meshFileRoute, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Mat(material) {}

	Model(Mesh* mesh, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(mesh), m_Mat(material) {}

	Model(const std::string meshFileRoute, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Mat(material) {}

	~Model() { 
		delete m_Mesh;
		delete m_Mat;
	}

	void setMesh(const std::string& pname);

	void setMaterial(Material* m);

	inline Material* getMaterialReference() { return m_Mat; }
	inline Mesh* getMesh() { return m_Mesh; }

	void draw(glm::mat4 proj, glm::mat4 view);

	Model* clone();

};