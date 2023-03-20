#pragma once
#include "VBO.h"
#include "VBLayout.h"


class VAO {
	unsigned int m_RendererID;
	unsigned int m_layoutCount;

public:
	VAO();

	~VAO();

	void addBuffer(const VBO& vbo, const VBLayout& layout);

	void bind() const;
	void unbind() const;
};
