#pragma once
#include "Mesh.h"
#include "SceneObject.h"


class Model : public SceneObject {
private:

	Mesh* m_Mesh;
	Material* m_Mat;

public:

	Model(const std::string na, Mesh* mesh) :SceneObject(na, ObjectType::MODEL), m_Mesh(mesh), m_Mat(nullptr) {}

	Model(const std::string na, Mesh* mesh, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(mesh), m_Mat(material) { m_Mesh->setMaterial(material); }

	Model(const std::string na, const std::string meshFileRoute, Material* material) : SceneObject(na, ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Mat(material) { m_Mesh->setMaterial(material); }

	Model(Mesh* mesh, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(mesh), m_Mat(material) { m_Mesh->setMaterial(material); }

	Model(const std::string meshFileRoute, Material* material) : SceneObject(ObjectType::MODEL), m_Mesh(new Mesh(meshFileRoute)), m_Mat(material) { m_Mesh->setMaterial(material); }

	Model() :SceneObject(ObjectType::MODEL), m_Mesh(nullptr), m_Mat(nullptr) {}

	Model(const std::string na) :SceneObject(na, ObjectType::MODEL), m_Mesh(nullptr), m_Mat(nullptr) {}

	~Model() {}

	void setMesh(const std::string& pname);

	void setMaterial(Material* m);

	inline Material* getMaterialReference() { return m_Mat; }
	inline Mesh* getMesh() { return m_Mesh; }

	void draw(glm::mat4 proj, glm::mat4 view);

	Model* clone();


};