#include "PhysicalMaterial.h"

void PhysicalMaterial::cacheUniforms()
{
	//Tiling
	m_shader->setFloat("u_TileV", m_TileV);
	m_shader->setFloat("u_TileU", m_TileU);
	m_shader->setBool("u_AlphaTest", m_Parameters.alphaTest);
	//Material properties
	if (m_AlbedoTex) {
		m_shader->setInt("material.albedoTex", 0);
		m_AlbedoTex->bind(0);
		m_shader->setBool("material.hasAlbedoTex", true);
	}
	else {
		m_shader->setBool("material.hasAlbedoTex", false);
		m_shader->setVec3("material.albedo", m_Albedo);
	}
	if (m_NormalTex) {
		m_shader->setInt("material.normalTex", 1);
		m_NormalTex->bind(1);
		m_shader->setBool("material.hasNormalTex", true);
	}
	else {
		m_shader->setBool("material.hasNormalTex", false);
	}
	if (m_RoughnessTex) {
		m_shader->setInt("material.roughnessTex", 2);
		m_RoughnessTex->bind(2);
		m_shader->setBool("material.hasRoughnessTex", true);
	}
	else {
		m_shader->setBool("material.hasRoughnessTex", false);
		m_shader->setFloat("material.roughness", m_Roughness);
	}
	if (presetType == 0)
		if (m_MetalnessTex) {
			m_shader->setInt("material.metalnessTex", 3);
			m_MetalnessTex->bind(3);
			m_shader->setBool("material.hasMetalnessTex", true);
		}
		else {
			m_shader->setBool("material.hasMetalnessTex", false);
			m_shader->setFloat("material.metalness", m_Metalness);
		}
	if (m_EmissiveTex) {
		m_shader->setInt("material.emissiveTex", 4);
		m_EmissiveTex->bind(4);
		m_shader->setBool("material.hasEmissiveTex", true);
	}
	else {
		m_shader->setBool("material.hasEmissiveTex", false);
		m_shader->setFloat("material.emissive", m_Emissive);
	}
	if (m_OpacityTex) {
		m_shader->setInt("material.opacityTex", 5);
		m_OpacityTex->bind(5);
		m_shader->setBool("material.hasOpacityTex", true);
	}
	else {
		m_shader->setBool("material.hasOpacityTex", false);
		m_shader->setFloat("material.opacity", m_Opacity);
	}
	if (presetType == 0)
		if (m_AOTex) {
			m_shader->setInt("material.AOTex", 6);
			m_AOTex->bind(6);
			m_shader->setBool("material.hasAOTex", true);
		}
		else {
			m_shader->setBool("material.hasAOTex", false);
			m_shader->setFloat("material.ao", m_AO);
		}

	m_shader->setInt("material.presetType", presetType);
	m_shader->setBool("material.receiveShadows", receiveShadows);
}

void PhysicalMaterial::decacheUniforms()
{
	if (m_AlbedoTex)		m_AlbedoTex->unbind();
	if (m_NormalTex)		m_NormalTex->unbind();
	if (m_RoughnessTex)		m_RoughnessTex->unbind();
	if (m_MetalnessTex)		m_MetalnessTex->unbind();
	if (m_EmissiveTex)		m_EmissiveTex->unbind();
	if (m_OpacityTex)		m_OpacityTex->unbind();
	if (m_AOTex)			m_AOTex->unbind();

}

void PhysicalMaterial::generateTextures()
{
	if (m_AlbedoTex)		m_AlbedoTex->generateTexture();
	if (m_NormalTex)		m_NormalTex->generateTexture();
	if (m_RoughnessTex)		m_RoughnessTex->generateTexture();
	if (m_MetalnessTex)		m_MetalnessTex->generateTexture();
	if (m_EmissiveTex)		m_EmissiveTex->generateTexture();
	if (m_OpacityTex)		m_OpacityTex->generateTexture();
	if (m_AOTex)			m_AOTex->generateTexture();

}
