#include "BasicPhongMaterial.h"




void BasicPhongMaterial::addColorTex(Texture* t) {
	colorTex = t;
	m_shader->bind();
	m_shader->setInt("material.colorTex", 0);
	m_shader->unbind();
}

void BasicPhongMaterial::addNormalTex(Texture* t) {
	normalTex = t;
	m_shader->bind();
	m_shader->setInt("material.normalTex", 1);
	m_shader->unbind();
}

void BasicPhongMaterial::addSpecularTex(Texture* t) {
	specularTex = t;
	m_shader->bind();
	m_shader->setInt("material.specularTex", 2);
	m_shader->unbind();
}

void BasicPhongMaterial::addRoughnessTex(Texture* t) {
	roughnessTex = t;
	m_shader->bind();
	m_shader->setInt("roughTex", 3);
	m_shader->unbind();
}

void BasicPhongMaterial::addOpacityTex(Texture* t)
{
	opacityTex = t;
	m_shader->bind();
	m_shader->setInt("opacityTex", 5);
	m_shader->unbind();
}

void BasicPhongMaterial::bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	m_shader->bind();

	glm::mat4 modelView = view * model;

	m_shader->setMat4("u_modelView", modelView);
	m_shader->setMat4("u_modelViewProj", proj * modelView);

	m_shader->setMat4("u_Model", model);
	m_shader->setFloat("u_TileV", itileV);
	m_shader->setFloat("u_TileU", itileU);

	if (colorTex != nullptr)
		colorTex->bind(0);
	if (normalTex != nullptr)
		normalTex->bind(1);
	if (specularTex != nullptr)
		specularTex->bind(2);
	if (roughnessTex != nullptr)
		roughnessTex->bind(3);
	if (emiTex != nullptr)
		emiTex->bind(4);
	if (opacityTex != nullptr)
		opacityTex->bind(5);

}

void BasicPhongMaterial::unbind()
{
	m_shader->unbind();
}

