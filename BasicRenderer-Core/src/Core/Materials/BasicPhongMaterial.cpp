#include "BasicPhongMaterial.h"

void BasicPhongMaterial::cacheUniforms()
{
	//Tiling
	m_shader->setFloat("u_TileV", m_TileV);
	m_shader->setFloat("u_TileU", m_TileU);

	//Mat properties
	if (m_ColorTex != nullptr) {
		m_shader->setInt("material.colorTex", 0);
		m_ColorTex->bind(0);
		m_shader->setBool("material.hasColorTex", true);
	}
	else {
		m_shader->setBool("material.hasColorTex", false);
		m_shader->setVec3("material.baseColor", m_BaseColor);
	}
	if (m_NormalTex != nullptr) {
		m_shader->setInt("material.normalTex", 1);
		m_NormalTex->bind(1);
		m_shader->setBool("material.hasNormalTex", true);
	}
	else {
		m_shader->setBool("material.hasNormalTex", false);
	}
	if (m_SpecularTex != nullptr) {
		m_shader->setInt("material.specularTex", 2);
		m_SpecularTex->bind(2);
		m_shader->setBool("material.hasSpecularTex", true);
	}
	else {
		m_shader->setBool("material.hasSpecularTex", false);
		m_shader->setFloat("material.specularity", m_Specularity);
	}
	if (m_ShininessTex != nullptr) {
		m_shader->setInt("material.glossTex", 3);
		m_ShininessTex->bind(3);
		m_shader->setBool("material.hasGlossTex", true);
	}
	else {
		m_shader->setBool("material.hasGlossTex", false);
		m_shader->setFloat("material.shininess", m_Shininess);
	}
	if (m_EmiTex != nullptr) {
		m_EmiTex->bind(4);
	}
	if (m_OpacityTex != nullptr) {
		m_shader->setInt("material.opacityTex", 5);
		m_OpacityTex->bind(5);
		m_shader->setBool("material.hasOpacityTex", true);
	}
	else {
		m_shader->setBool("material.hasOpacityTex", false);
		m_shader->setFloat("material.opacity", m_Opacity);
	}
	m_shader->setBool("material.receiveShadows", receiveShadows);

}

void BasicPhongMaterial::decacheUniforms()
{
	if (m_ColorTex != nullptr) {
		m_ColorTex->unbind();
	}
	if (m_NormalTex != nullptr) {
		m_NormalTex->unbind();
	}
	if (m_SpecularTex != nullptr) {
		m_SpecularTex->unbind();
	}
	if (m_ShininessTex != nullptr) {
		m_ShininessTex->unbind();
	}
	if (m_EmiTex != nullptr) {
		m_EmiTex->unbind();
	}
	if (m_OpacityTex != nullptr) {
		m_OpacityTex->unbind();
	}
	//m_shader->unbind();
}

void BasicPhongMaterial::generateTextures()
{

	if (m_ColorTex != nullptr) {
		m_ColorTex->generateTexture();
	}
	if (m_NormalTex != nullptr) {
		m_NormalTex->generateTexture();
	}
	if (m_SpecularTex != nullptr) {
		m_SpecularTex->generateTexture();
	}
	if (m_ShininessTex != nullptr) {
		m_ShininessTex->generateTexture();
	}
	if (m_EmiTex != nullptr) {
		m_EmiTex->generateTexture();
	}
	if (m_OpacityTex != nullptr) {
		m_OpacityTex->generateTexture();
	}

}

