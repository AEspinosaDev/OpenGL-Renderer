#include "SkyboxMaterial.h"

void SkyboxMaterial::cacheUniforms()
{
	m_shader->bind();


	if (cubeMapText) {
		cubeMapText->bind(0);
		m_shader->setInt("u_cubeMapText", 0);
	}

}

void SkyboxMaterial::decacheUniforms()
{
	m_shader->unbind();
}

void SkyboxMaterial::generateTextures()
{
	if (cubeMapText) {
		cubeMapText->generateTexture();
	}
}
