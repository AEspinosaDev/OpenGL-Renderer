#include "Model.h"

void Model::loadMesh(const std::string& pname) {
	if (m_Mesh != nullptr)
		delete m_Mesh;
	m_Mesh = new Mesh();
	m_Mesh->importFile(pname);
	
}

void Model::setPosition(glm::vec3  p) {
	SceneObject::setPosition(p);
	if (m_Mesh != nullptr) {
		m_Mesh->setPos(p);
	}
}
glm::vec3  Model::getPosition() {

	if (m_Mesh != nullptr)
		std::cout << m_Name << " position is " << m_Location.x << ", " << m_Location.y << ", " << m_Location.z << "." << std::endl;
	SceneObject::getPosition();
	return m_Location;

}
void Model::setScale(const float  s) {
	SceneObject::setScale(s);
	if (m_Mesh != nullptr) {
		m_Mesh->setSize(s);
	}


}

void Model::setRotation(const glm::vec3 p)
{
	if (m_Mesh != nullptr)
		m_Mesh->setRotation(p);
}


void Model::loadMaterial(Material* m) {
	if (m_Mat != nullptr)
		delete m_Mat;
	m_Mat = m;
	if (m_Mesh != nullptr)
		m_Mesh->setMaterial(m);
	else
		std::cout << "Model doesnt have any mesh loaded" << std::endl;
}

void Model::draw(glm::mat4 proj, glm::mat4 view) {
	if (!isActive()) return;

	if (m_Mesh != nullptr && m_Mat != nullptr)
		m_Mesh->draw(proj,view);
	else
		std::cout << "Model doesnt have any mesh loaded" << std::endl;
}
