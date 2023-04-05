#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture(CubeMapFaces paths) : Texture(), m_FacesPath(paths)
{
	generateCubeMapTexture(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

CubeMapTexture::CubeMapTexture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, int magFilter, int minFilter, int wrapT, int wrapS, int wrapR):Texture(), m_FacesPath(CubeMapFaces("", "", "", "", "", ""))
{
	m_Width = w; m_Height = h;

	this->level = level;
	this->internalFormat = internalFormat;
	this->border = border;
	this->format = format;
	this->type = type;

	glGenTextures(1, &m_RendererID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, internalFormat,
			w, h, border, format, type, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);

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

void CubeMapTexture::generateCubeMapTexture(int magFilter, int minFilter, int wrapT, int wrapS, int wrapR)
{
	std::string faces[] = {
		texturePath + m_FacesPath.right,
		texturePath + m_FacesPath.left,
		texturePath + m_FacesPath.top,
		texturePath + m_FacesPath.bottom,
		texturePath + m_FacesPath.back,
		texturePath + m_FacesPath.front
	};

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

	for (unsigned int i = 0; i < NUM_FACES; i++)
	{
		int width , height, nrChannels;
		m_FilePath = faces[i];
		m_LocalBuffer = stbi_load(m_FilePath.c_str(), &width, &height, &nrChannels, 0);
		if (m_LocalBuffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, m_LocalBuffer);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
		stbi_image_free(m_LocalBuffer);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);

	std::cout << "Cubemap loaded" << std::endl;
}
