#pragma once
#include "Mesh.h"
#include "SceneObject.h"


class Model : public SceneObject {
private:

	Mesh* m_Mesh;
	Material* m_Mat;

public:

	Model(const std::string na, Mesh* me) :SceneObject(na, ObjectType::MODEL ), m_Mesh(me), m_Mat(nullptr) {}

	Model(const std::string na, Mesh* me, Material* ma) : SceneObject(na, ObjectType::MODEL), m_Mesh(me), m_Mat(ma) {}

	Model() :SceneObject(ObjectType::MODEL), m_Mesh(nullptr), m_Mat(nullptr) {}

	Model(const std::string na) :SceneObject(na, ObjectType::MODEL), m_Mesh(nullptr), m_Mat(nullptr) {}


	~Model() {}

	void loadMesh(const std::string& pname);

	void loadMaterial(Material* m);

	inline Material* getMaterialReference() { return m_Mat; }
	inline Mesh* getMesh() { return m_Mesh; }

	void draw(glm::mat4 proj, glm::mat4 view);

	Model* clone();


};