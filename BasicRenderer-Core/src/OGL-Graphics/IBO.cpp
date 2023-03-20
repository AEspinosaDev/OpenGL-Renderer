#include "IBO.h"

	
IBO::IBO(const unsigned int* data, unsigned int count) : m_Count(count) {

	ASSERT(sizeof(GLuint) == sizeof(unsigned int));
	GLcall(glGenBuffers(1, &m_RendererID));
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IBO::bind() const {
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IBO::unbind() const {
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}