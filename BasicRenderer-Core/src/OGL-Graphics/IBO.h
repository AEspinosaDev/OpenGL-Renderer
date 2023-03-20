#pragma once
#include "Graphics_Core.h"


//Index buffer object

class IBO {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IBO(const unsigned int* data, unsigned int count); 

	~IBO() {
		//GLcall(glDeleteBuffers(1, &m_RendererID))
	}

	void bind() const;

	void unbind() const;

	inline unsigned int getCount() const { return m_Count; }
	inline unsigned int getID() const { return m_RendererID; }

};