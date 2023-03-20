#include "Material.h"

Material::Material(Shader* shader, const std::string& colorPath, const std::string& normalPath, const std::string& specularPath, const std::string& roughnessPath, const std::string& emiPath) : m_shader(shader), itileU(1), itileV(1) {

	m_shader->bind();
	colorTex = new Texture(colorPath);
	m_shader->setInt("material.colorTex", 0);
	normalTex = new Texture(normalPath);
	m_shader->setInt("material.normalTex", 1);
	specularTex = new Texture(specularPath);
	m_shader->setInt("material.specularTex", 2);
	roughnessTex = new Texture(roughnessPath);
	m_shader->setInt("material.glossTex", 3);
	emiTex = new Texture(emiPath);
	m_shader->setInt("emiTex", 4);
	m_shader->unbind();

}

Material::Material(Shader* shader): m_shader(shader), itileU(1), itileV(1) {

	colorTex = nullptr;
	normalTex = nullptr;
	specularTex = nullptr;
	roughnessTex = nullptr;
	emiTex = nullptr;

}
void Material::bindTextures() {

	m_shader->bind();
	

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

}

void Material::addColorTex(Texture* t) {
	colorTex = t;
	m_shader->bind();
	m_shader->setInt("material.colorTex", 0);
	m_shader->unbind();
}

void Material::addNormalTex(Texture* t) {
	normalTex = t;
	m_shader->bind();
	m_shader->setInt("material.normalTex", 1);
	m_shader->unbind();
}

void Material::addSpecularTex(Texture* t) {
	specularTex = t;
	m_shader->bind();
	m_shader->setInt("material.specularTex", 2);
	m_shader->unbind();
}

void Material::addRoughnessTex(Texture* t) {
	roughnessTex = t;
	m_shader->bind();
	m_shader->setInt("roughTex", 3);
	m_shader->unbind();
}

