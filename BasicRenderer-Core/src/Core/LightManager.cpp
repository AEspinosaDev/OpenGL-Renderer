#include "LightManager.h"
#include <Core/DirectionalLight.h>

void LightManager::init(Shader* s)
{
	m_PLightMesh = new Mesh();
	m_PLightMesh->importFile("Sphere.obj");

	m_SLightMesh = new Mesh();
	m_SLightMesh->importFile("Sphere.obj");

	m_DebugMat = new Material(s);
	m_PLightMesh->setMaterial(m_DebugMat);
}

void LightManager::addLight(Light* l) {

	switch (l->getType())
	{
	case 0:
		pointLightsNumber++;
		break;
	case 1:
		directionalLightsNumber++;
		break;
	case 2:
		spotLightsNumber++;
		break;
	}
	m_Lights.push_back(l);
}

void LightManager::removeLight()
{
	//m_Lights
}

void LightManager::drawLights(glm::mat4 proj, glm::mat4 view) {
	for (Light* l : m_Lights)
	{
		Shader* s = m_DebugMat->getShader();
		s->bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), l->getPosition());
		model = glm::scale(model, glm::vec3(0.5));
		glm::mat4 modelView = view * model;
		s->setMat4("u_modelViewProj", proj * modelView);
		s->setVec3("u_color", l->getColor());
		s->unbind();

		switch (l->getType())
		{
		case 0:
			m_PLightMesh->draw();
			break;
		
		case 2:
			m_SLightMesh->draw();
			break;
		}

	}
}

void LightManager::uploadLightDataToShader(Shader* s, glm::mat4 view)
{
	s->bind();

	for (size_t i = 0; i < m_Lights.size(); i++)
	{
		if (m_Lights[i]->isActive()) {

			switch (m_Lights[i]->getType())
			{

			case 0:
				s->setVec3("pointLights[" + std::to_string(i) + "].pos", view * glm::vec4(m_Lights[i]->getPosition(), 1.0));
				s->setVec3("pointLights[" + std::to_string(i) + "].color", m_Lights[i]->getColor());
				s->setFloat("pointLights[" + std::to_string(i) + "].intensity", m_Lights[i]->getIntensity());
				break;
			case 1:
				s->setVec3("directionalLights[" + std::to_string(i) + "].dir", view * glm::vec4(static_cast<DirectionalLight*>(m_Lights[i])->getDirection(), 0.0));
				s->setVec3("directionalLights[" + std::to_string(i) + "].color", m_Lights[i]->getColor());
				s->setFloat("directionalLights[" + std::to_string(i) + "].intensity", m_Lights[i]->getIntensity());
				break;
			case 2:
				break;
			}
		}

	}
	s->setInt("pointsLightsNumber", pointLightsNumber);
	s->setInt("directionalLightsNumber", directionalLightsNumber);
	s->setInt("spotLightsNumber", spotLightsNumber);

	s->unbind();



}
