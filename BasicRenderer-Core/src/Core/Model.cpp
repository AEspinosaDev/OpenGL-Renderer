#include "Model.h"



void Model::setMesh(const std::string& pname) {
	if (m_Mesh != nullptr)
		delete m_Mesh;
	m_Mesh = new Mesh(pname);
	//m_Mesh->importFile();
	
}


void Model::setMaterial(Material* m) {
	if (m_Mat != nullptr)
		delete m_Mat;
	m_Mat = m;
	
}

void Model::draw(glm::mat4 proj, glm::mat4 view) {
	if (!isActive()) return;

	if (m_Mesh != nullptr && m_Mat != nullptr) {
		m_Mat->setupParameters();

		m_Mat->bind(proj, view, m_Transform);

		m_Mesh->draw();

		m_Mat->unbind();
	}
	else
		std::cout << "Model doesnt have any mesh loaded" << std::endl;
}


Model* Model::clone()
{
	m_Clones++;
	return new Model(getName()+"clone"+std::to_string(m_Clones-1), m_Mesh, m_Mat);
}
