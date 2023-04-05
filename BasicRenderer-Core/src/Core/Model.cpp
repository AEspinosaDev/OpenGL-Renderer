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

void Model::drawDepth(Shader* depthShader, glm::mat4 viewProjMatrix)
{
	if (!isActive()) return;

	if (m_Mesh != nullptr) {
		m_Mesh->setModel(m_Transform);
		m_Mesh->drawShadows(depthShader, viewProjMatrix);
	}
	else
		std::cout << "Model doesnt have any mesh loaded" << std::endl;
}

Model* Model::clone()
{
	return new Model("", m_Mesh, m_Mat);
}
