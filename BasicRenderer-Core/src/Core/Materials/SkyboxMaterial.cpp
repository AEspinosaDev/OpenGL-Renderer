#include "SkyboxMaterial.h"

void SkyboxMaterial::cacheUniforms()
{
	m_shader->bind();


	if (m_CubeMapText) {
		m_CubeMapText->bind(0);
		m_shader->setInt("u_cubeMapText", 0);
	}

}

void SkyboxMaterial::decacheUniforms()
{
	m_shader->unbind();
}

void SkyboxMaterial::generateTextures()
{
	if (m_CubeMapText) {
		m_CubeMapText->generateTexture();
	}
}
