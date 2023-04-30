#include "CubeMapTexture.h"

CubeMapTexture::CubeMapTexture(CubeMapFaces paths) :

	Texture(0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE,
		false, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE),

	m_FacesPath(paths)
{
	m_TextureType = TextureType::TEXTURE_CUBE;
}

CubeMapTexture::CubeMapTexture(GLint level, GLint internalFormat, unsigned int w, unsigned int h, GLint border, GLenum format, GLenum type, int magFilter, int minFilter, int wrapT, int wrapS, int wrapR) :

	Texture(level, internalFormat, w, h, border, format, type, false, magFilter, minFilter, wrapT, wrapS),

	m_FacesPath(CubeMapFaces("null", "null", "null", "null", "null", "null"))
{
	m_TextureType = TextureType::TEXTURE_CUBE;
	m_TextConfig.wrapR = wrapR;
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

void CubeMapTexture::generateTexture()
{
	if (!isDirty) return;

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

	if (m_FacesPath.right != "null") {
		std::string faces[] = {
			texturePath + m_FacesPath.right,
			texturePath + m_FacesPath.left,
			texturePath + m_FacesPath.top,
			texturePath + m_FacesPath.bottom,
			texturePath + m_FacesPath.back,
			texturePath + m_FacesPath.front
		};


		for (unsigned int i = 0; i < NUM_FACES; i++)
		{
			int width, height, nrChannels;
			m_FilePath = faces[i];
			m_LocalBuffer = stbi_load(m_FilePath.c_str(), &width, &height, &nrChannels, 0);
			if (m_LocalBuffer)
			{
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					m_TextConfig.level,
					m_TextConfig.internalFormat,
					width,
					height,
					m_TextConfig.border,
					m_TextConfig.format,
					m_TextConfig.type,
					m_LocalBuffer
				);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			}
			stbi_image_free(m_LocalBuffer);
		}
		std::cout << "Cubemap loaded" << std::endl;
	}
	else {
		for (unsigned int i = 0; i < NUM_FACES; ++i)

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				m_TextConfig.level,
				m_TextConfig.internalFormat,
				m_Width,
				m_Height,
				m_TextConfig.border,
				m_TextConfig.format,
				m_TextConfig.type,
				NULL
			);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_TextConfig.minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_TextConfig.magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_TextConfig.wrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_TextConfig.wrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_TextConfig.wrapR);

	isDirty = false;
}


