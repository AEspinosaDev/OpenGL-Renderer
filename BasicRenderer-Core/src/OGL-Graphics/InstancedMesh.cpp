#include "InstancedMesh.h"

void InstancedMesh::setWorldMatrices(std::vector<glm::mat4> matrices) {
	for (size_t i = 0; i < m_InstanceCount; i++)
	{
		m_WorldMatrices[i] = matrices[i];
	}
}

void InstancedMesh::draw() {
	m_Vao.bind();
	GLcall(glDrawElementsInstanced(GL_TRIANGLES, m_Triangles, GL_UNSIGNED_INT, (void*)0, m_InstanceCount));
}

void InstancedMesh::generateBuffers() {
	Mesh::generateBuffers();

	m_Vao.bind();

	VBO instanceMatrix_b(&m_WorldMatrices[0], m_InstanceCount * sizeof(glm::mat4));

	// vertex attributes
	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);


	m_Vao.unbind();


}