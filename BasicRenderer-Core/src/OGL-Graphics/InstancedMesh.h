#pragma once
#include "Mesh.h"


class InstancedMesh : public Mesh {
private:
	unsigned int m_InstanceCount;
	glm::mat4* m_WorldMatrices;

public:
	InstancedMesh(std::string path, unsigned int instance_count);

	inline unsigned int getInstanceCount() { return m_InstanceCount; }

	//inline void setInstanceCount(unsigned int c) { m_InstanceCount = c; }

	inline glm::mat4* getWorldMatrices() {	return m_WorldMatrices;}

	void setWorldMatrices(glm::mat4* matrices);

	void generateBuffers();

	void draw();

	void draw(int meshIdx);
};