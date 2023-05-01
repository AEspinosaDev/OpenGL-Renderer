#include "Model.h"





void Model::setMesh(const std::string& pname) {
	
	m_Mesh = new Mesh(pname);
	//m_Mesh->importFile();

}


void Model::setMaterial(Material* m, unsigned int id) {
	
	m_Materials[id] = m;

}

//void Model::draw() {
//	if (!isActive()) return;
//
//	if (m_Mesh != nullptr) {
//		for (size_t i = 0; i < m_Mesh->getNumberOfMeshes(); i++)
//		{
//			m_Materials[i]->setupParameters();
//
//			m_Materials[i]->bind();
//
//			m_Mesh->draw(i);
//
//			m_Materials[i]->unbind();
//		}
//	}
//	else
//		std::cout << "Model doesnt have any mesh loaded" << std::endl;
//}


Model* Model::clone()
{
	m_Clones++;
	return new Model(getName() + "clone" + std::to_string(m_Clones - 1), m_Mesh, m_Materials[0]);
}
