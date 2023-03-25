#include "UnlitBasicMaterial.h"


void UnlitBasicMaterial::addColorTex(Texture* t)
{

	colorTex = t;
	m_shader->bind();
	m_shader->setInt("u_colorTex", 0);
	m_shader->unbind();

}

void UnlitBasicMaterial::bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	m_shader->bind();
	glm::mat4 modelView = view * model;
	m_shader->setMat4("u_modelViewProj", proj * modelView);
	m_shader->setFloat("u_TileV", itileV);
	m_shader->setFloat("u_TileU", itileU);

	if (colorTex != nullptr)
		colorTex->bind(0);
	else
		m_shader->setVec3("u_color", color);
}

void UnlitBasicMaterial::unbind()
{
	m_shader->unbind();
}


