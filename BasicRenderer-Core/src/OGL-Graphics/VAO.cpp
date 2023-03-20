#include"VAO.h"

VAO::VAO() : m_layoutCount(0) {
	GLcall(glGenVertexArrays(1, &m_RendererID));

}

VAO::~VAO() {
	GLcall(glDeleteVertexArrays(1, &m_RendererID));
}

void VAO::addBuffer(const VBO& vbo, const VBLayout& layout) {
	bind();
	vbo.bind();
	unsigned int offset = 0;
	const auto& elements = layout.getElements();

	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLcall(glEnableVertexAttribArray(m_layoutCount));
		GLcall(glVertexAttribPointer(m_layoutCount, element.count, element.type,
			element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VBElement::getSizeOf(element.type);
		m_layoutCount++;
	}
}

void VAO::bind() const {

	GLcall(glBindVertexArray(m_RendererID));
}
void VAO::unbind() const {

	GLcall(glBindVertexArray(0));
}