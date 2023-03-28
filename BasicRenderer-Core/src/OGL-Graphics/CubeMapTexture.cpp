#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture(CubeMapFaces paths) : Texture(), m_FacesPath(paths)
{
	generateCubeMapTexture(GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void CubeMapTexture::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void CubeMapTexture::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMapTexture::generateCubeMapTexture( int magFilter, int minFilter, int wrapT, int wrapS, int wrapR)
{
	std::string faces[] = {
		m_FacesPath.right,
		m_FacesPath.left,
		m_FacesPath.top,
		m_FacesPath.bottom,
		m_FacesPath.back,
		m_FacesPath.front
	};

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

	for (unsigned int i = 0; i < NUM_FACES; i++)
	{
		m_FilePath = faces[i];
		//load data
		m_LocalBuffer = loadTexture(m_FilePath.c_str(), m_Width, m_Height);
		if (m_LocalBuffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, internalFormat, m_Width, m_Height, border, format,
				type, (GLvoid*)m_LocalBuffer);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
		delete[] m_LocalBuffer; 
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);


}
