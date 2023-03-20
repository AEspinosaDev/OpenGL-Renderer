#include "VBO.h"

VBO::VBO(const void* data, unsigned int size) {

	GLcall(glGenBuffers(1, &m_RendererID));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLcall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}


void VBO::bind() const {
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VBO::unbind() const {
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}