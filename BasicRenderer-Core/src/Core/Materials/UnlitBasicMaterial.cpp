#include "UnlitBasicMaterial.h"




void UnlitBasicMaterial::cacheUniforms()
{
	//m_shader->bind();

	m_shader->setFloat("u_TileV", m_TileV);
	m_shader->setFloat("u_TileU", m_TileU);
	m_shader->setFloat("u_opacity", m_Opacity);
	m_shader->setFloat("u_overrideColorStrength", m_OverrideColorStr);
	m_shader->setBool("u_overrideColor", m_OverrideColor);
	m_shader->setBool("u_AlphaTest", m_Parameters.alphaTest);


	if (m_ColorText != nullptr) {
		m_shader->setInt("u_colorTex", 0);
		m_ColorText->bind(0);
		m_shader->setBool("u_hasColorTex", true);
	}
	else {
		m_shader->setBool("u_hasColorTex", false);
		m_shader->setVec3("u_color", m_Color);
	}
	if (m_OpacityText != nullptr) {
		m_shader->setInt("u_opacityTex", 1);
		m_OpacityText->bind(1);
		m_shader->setBool("u_hasOpacityTex", true);
	}
	else {
		m_shader->setBool("u_hasOpacityTex", false);
		m_shader->setFloat("u_opacity", m_Opacity);
	}


}

void UnlitBasicMaterial::decacheUniforms()
{
	if (m_ColorText != nullptr) {
		m_ColorText->unbind();
		m_shader->setBool("u_hasColorTex", false);
	}
	if (m_OpacityText != nullptr) {
		m_OpacityText->unbind();
		m_shader->setBool("u_hasOpacityTex", false);
	}
	//m_shader->unbind();
}

void UnlitBasicMaterial::generateTextures()
{
	if (m_ColorText != nullptr) {
		m_ColorText->generateTexture();
	}
	if (m_OpacityText != nullptr) {
		m_OpacityText->generateTexture();
	}
}


