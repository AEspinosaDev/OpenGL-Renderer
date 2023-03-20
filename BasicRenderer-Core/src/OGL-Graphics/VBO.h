#pragma once
#include "Graphics_Core.h"



//Vertex buffer object

class VBO {
private:
	unsigned int m_RendererID;
public:
	VBO(const void* data, unsigned int size);

	~VBO() {
		//GLcall(glDeleteBuffers(1, &m_RendererID))
	}

	void bind() const;

	void unbind() const;
};