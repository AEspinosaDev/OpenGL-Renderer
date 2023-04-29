#pragma once
#include "Mesh.h"


class InstancedMesh : public Mesh {
private:
	unsigned int m_InstanceCount;
	glm::mat4* m_WorldMatrices;

public:
	InstancedMesh(std::string path, unsigned int instance_count) : Mesh(path), m_InstanceCount(instance_count), m_WorldMatrices(new glm::mat4[instance_count]) {}

	inline unsigned int getInstanceCount() { return m_InstanceCount; }

	inline void setInstanceCount(unsigned int c) { m_InstanceCount = c; }

	inline glm::mat4* getWorldMatrices() {	return m_WorldMatrices;}

	inline void setWorldMatrices(std::vector<glm::mat4> matrices);

	void generateBuffers();

	void draw();


};