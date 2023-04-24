#include "BasicPhongMaterial.h"




void BasicPhongMaterial::addColorTex(Texture* t) {
	colorTex = t;
}

void BasicPhongMaterial::addNormalTex(Texture* t) {
	normalTex = t;
}


void BasicPhongMaterial::addSpecularTex(Texture* t) {
	specularTex = t;
}


void BasicPhongMaterial::addRoughnessTex(Texture* t) {
	roughnessTex = t;
}


void BasicPhongMaterial::addOpacityTex(Texture* t)
{
	opacityTex = t;
}


void BasicPhongMaterial::bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	m_shader->bind();

	glm::mat4 modelView = view * model;
	//Matrices
	m_shader->setMat4("u_modelView", modelView);
	m_shader->setMat4("u_modelViewProj", proj * modelView);
	m_shader->setMat4("u_Model", model);
	//Tiling
	m_shader->setFloat("u_TileV", itileV);
	m_shader->setFloat("u_TileU", itileU);


	//Mat properties
	if (colorTex != nullptr) {
		m_shader->setInt("material.colorTex", 0);
		colorTex->bind(0);
		m_shader->setBool("material.hasColorTex", true);
	}
	else {
		m_shader->setBool("material.hasColorTex", false);
		m_shader->setVec3("material.baseColor", baseColor);
	}
	if (normalTex != nullptr) {
		m_shader->setInt("material.normalTex", 1);
		normalTex->bind(1);
		m_shader->setBool("material.hasNormalTex", true);
	}
	else {
		m_shader->setBool("material.hasNormalTex", false);
	}
	if (specularTex != nullptr) {
		m_shader->setInt("material.specularTex", 2);
		specularTex->bind(2);
		m_shader->setBool("material.hasSpecularTex", true);
	}
	else {
		m_shader->setBool("material.hasSpecularTex", false);
		m_shader->setFloat("material.specularity", specularity);
	}
	if (roughnessTex != nullptr) {
		m_shader->setInt("material.glossTex", 3);
		roughnessTex->bind(3);
		m_shader->setBool("material.hasGlossTex", true);
	}
	else {
		m_shader->setBool("material.hasGlossTex", false);
		m_shader->setFloat("material.shininess", shininess);
	}
	if (emiTex != nullptr) {
		emiTex->bind(4);
	}
	if (opacityTex != nullptr) {
		m_shader->setInt("material.opacityTex", 5);
		opacityTex->bind(5);
		m_shader->setBool("material.hasOpacityTex", true);
	}
	else {
		m_shader->setBool("material.hasOpacityTex", false);
		m_shader->setFloat("material.opacity", opacity);
	}
	m_shader->setBool("material.receiveShadows", receiveShadows);

}

void BasicPhongMaterial::unbind()
{
	if (colorTex != nullptr) {
		colorTex->unbind();
	}
	if (normalTex != nullptr) {
		normalTex->unbind();
	}
	if (specularTex != nullptr) {
		specularTex->unbind();
	}
	if (roughnessTex != nullptr) {
		roughnessTex->unbind();
	}
	if (emiTex != nullptr) {
		emiTex->unbind();
	}
	if (opacityTex != nullptr) {
		opacityTex->unbind();
	}
	m_shader->unbind();
}

