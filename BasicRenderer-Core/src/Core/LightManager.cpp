#include "LightManager.h"
#include <Core/Lights/DirectionalLight.h>
#include <CubeMapTexture.h>

void LightManager::init(std::unordered_map<std::string, Shader*> shaders)
{
	m_PLightMesh = new Mesh();
	m_PLightMesh->importFile("Sphere.obj");

	m_SLightMesh = new Mesh();
	m_SLightMesh->importFile("Sphere.obj");

	m_DLightMesh = new Mesh();
	m_DLightMesh->importFile("cylinder.obj");

	m_DebugMat = new UnlitBasicMaterial(shaders);
	m_PLightMesh->setMaterial(m_DebugMat);
	m_SLightMesh->setMaterial(m_DebugMat);
	m_DLightMesh->setMaterial(m_DebugMat);
}

void LightManager::addLight(Light* l) {

	if (lightsNumber == MAX_LIGHTS) return;

	switch (l->getType())
	{
	case 0:
		pointLightsNumber++;
		l->setShadowText(new CubeMapTexture(0, GL_DEPTH_COMPONENT, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE));
		/*l->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));*/
		break;
	case 1:
		directionalLightsNumber++;
		l->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
		break;
	case 2:
		spotLightsNumber++;
		l->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
		break;
	}
	lightsNumber++;
	m_Lights.push_back(l);
}

void LightManager::removeLight()
{
	//m_Lights
}

void LightManager::drawLights(glm::mat4 proj, glm::mat4 view) {
	for (Light* l : m_Lights)
	{

		glm::mat4 model = glm::translate(glm::mat4(1.0f), l->getPosition());
		model = glm::scale(model, glm::vec3(0.5));

		m_DebugMat->setColor(l->getColor());

		switch (l->getType())
		{
		case 0:
			m_PLightMesh->setModel(model);
			m_PLightMesh->draw(proj, view);
			break;
		case 1:
			m_DLightMesh->setModel(model);
			m_DLightMesh->draw(proj, view);
			break;
		case 2:
			m_SLightMesh->setModel(model);
			m_SLightMesh->draw(proj, view);
			break;
		}

	}
}

void LightManager::uploadLightDataToShader(Shader* s, glm::mat4 view)
{
	s->bind();

	int pointLightIndex = 0;
	int dirLightIndex = 0;
	int spotLightIndex = 0;
	int textureSlotIndex = 6;

	for (size_t i = 0; i < m_Lights.size(); i++)
	{
		if (m_Lights[i]->isActive()) {

			switch (m_Lights[i]->getType())
			{

			case 0:

				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].pos", view * glm::vec4(m_Lights[i]->getPosition(), 1.0));
				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].worldPos", m_Lights[i]->getPosition());
				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].color", m_Lights[i]->getColor());
				s->setFloat("pointLights[" + std::to_string(pointLightIndex) + "].intensity", m_Lights[i]->getIntensity());
				s->setBool("pointLights[" + std::to_string(pointLightIndex) + "].castShadows", m_Lights[i]->getCastShadows());
				if (m_Lights[i]->getCastShadows()) {
					s->setMat4("pointLights[" + std::to_string(pointLightIndex) + "].lightViewProj", m_Lights[i]->getLightTransformMatrix(glm::vec3(0.0f)));
					m_Lights[i]->getShadowText()->bind(textureSlotIndex);
					s->setInt("pointLights[" + std::to_string(pointLightIndex) + "].shadowMap", textureSlotIndex);
					textureSlotIndex++;
				}
				pointLightIndex++;
				break;
			case 1:
				s->setVec3("directionalLights[" + std::to_string(dirLightIndex) + "].dir", view * glm::vec4(static_cast<DirectionalLight*>(m_Lights[i])->getDirection(), 0.0));
				s->setVec3("directionalLights[" + std::to_string(dirLightIndex) + "].color", m_Lights[i]->getColor());
				s->setFloat("directionalLights[" + std::to_string(dirLightIndex) + "].intensity", m_Lights[i]->getIntensity());
				s->setBool("directionalLights[" + std::to_string(dirLightIndex) + "].castShadows", m_Lights[i]->getCastShadows());
				if (m_Lights[i]->getCastShadows()) {
					s->setMat4("directionalLights[" + std::to_string(dirLightIndex) + "].lightViewProj", m_Lights[i]->getLightTransformMatrix(glm::vec3(0.0f)));
					m_Lights[i]->getShadowText()->bind(textureSlotIndex);
					s->setInt("directionalLights[" + std::to_string(dirLightIndex) + "].shadowMap", textureSlotIndex);
					textureSlotIndex++;
				}
				dirLightIndex++;
				break;
			case 2:
				break;
			}
		}

	}

	s->setFloat("u_ambientStrength", m_AmbientStrength);
	s->setVec3("u_ambientColor", m_AmbientColor);

	s->setInt("pointsLightsNumber", pointLightsNumber);
	s->setInt("directionalLightsNumber", directionalLightsNumber);
	s->setInt("spotLightsNumber", spotLightsNumber);

	s->unbind();

}

