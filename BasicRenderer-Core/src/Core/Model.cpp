#include "Model.h"



void Model::loadMesh(const std::string& pname) {
	if (m_Mesh != nullptr)
		delete m_Mesh;
	m_Mesh = new Mesh();
	m_Mesh->importFile(pname);
	
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

	if (m_Mesh != nullptr && m_Mat != nullptr) {
		m_Mesh->setModel(m_Transform);
		m_Mesh->setMaterial(m_Mat);
		m_Mesh->draw(proj, view);
	}
	else
		std::cout << "Model doesnt have any mesh loaded" << std::endl;
}


Model* Model::clone()
{
	m_Clones++;
	//Ask for the last symbol of the name, if its a number increase it by one and change it
	return new Model(getName()+"clone"+std::to_string(m_Clones-1), m_Mesh, m_Mat);
}
