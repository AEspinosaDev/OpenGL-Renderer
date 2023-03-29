#include "SkyboxMaterial.h"

void SkyboxMaterial::bind(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
{
	m_shader->bind();

	//glm::mat4 modelView = view * model;

	m_shader->setMat4("u_viewProj", proj * view);

	if (cubeMapText) {
		cubeMapText->bind(0);
		m_shader->setInt("u_cubeMapText", 0);
	}

}

void SkyboxMaterial::unbind()
{
	m_shader->unbind();
}
