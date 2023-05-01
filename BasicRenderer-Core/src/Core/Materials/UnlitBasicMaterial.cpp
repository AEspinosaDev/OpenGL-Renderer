#include "UnlitBasicMaterial.h"


void UnlitBasicMaterial::addColorTex(Texture* t)
{

	colorTex = t;


}

void UnlitBasicMaterial::cacheUniforms()
{
	//m_shader->bind();
	
	m_shader->setFloat("u_TileV", itileV);
	m_shader->setFloat("u_TileU", itileU);
	m_shader->setFloat("u_opacity", opacity);

	if (colorTex != nullptr) {
		m_shader->setInt("u_colorTex", 0);
		colorTex->bind(0);
	}
	else
		m_shader->setVec3("u_color", color);
}

void UnlitBasicMaterial::decacheUniforms()
{
	if (colorTex != nullptr) {
		colorTex->unbind();
	}
	//m_shader->unbind();
}

void UnlitBasicMaterial::generateTextures()
{
	if (colorTex != nullptr) {
		colorTex->generateTexture();
	}
}


