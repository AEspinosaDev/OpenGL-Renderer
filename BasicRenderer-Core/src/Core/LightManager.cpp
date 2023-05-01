#include "LightManager.h"
#include <Core/Lights/DirectionalLight.h>
#include <CubeMapTexture.h>

void LightManager::init()
{
	m_DebugMat = new UnlitBasicMaterial();

	m_PLightMesh = new Model("Sphere.obj", m_DebugMat);
	m_PLightMesh->setScale(glm::vec3(0.5f));
	m_PLightMesh->getMesh()->generateBuffers();
	m_SLightMesh = new Model("Sphere.obj", m_DebugMat);
	m_SLightMesh->setScale(glm::vec3(0.5f));
	m_SLightMesh->getMesh()->generateBuffers();
	m_DLightMesh = new Model("cylinder.obj", m_DebugMat);
	m_DLightMesh->setScale(glm::vec3(0.5f));
	m_DLightMesh->getMesh()->generateBuffers();
}

void LightManager::addLight(Light* l) {

	if (lightsNumber == MAX_LIGHTS) return;
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	switch (l->getType())
	{
	case 0:
		pointLightsNumber++;
		l->setShadowText(new CubeMapTexture(0, GL_DEPTH_COMPONENT, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE));

		break;
	case 1:
		directionalLightsNumber++;
		l->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
		GLcall(glBindTexture(GL_TEXTURE_2D, l->getShadowText()->getID()));
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	case 2:
		spotLightsNumber++;
		l->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, m_ShadowResolution, m_ShadowResolution, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
		GLcall(glBindTexture(GL_TEXTURE_2D, l->getShadowText()->getID()));
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));
		break;
	}
	lightsNumber++;
	m_Lights.push_back(l);
}

void LightManager::removeLight()
{
	//m_Lights
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
					s->setMat4("directionalLights[" + std::to_string(dirLightIndex) + "].lightViewProj", m_Lights[i]->getLightTransformMatrix());
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
	s->setFloat("u_shadowsFarPlane", m_ShadowFarPlane);
	s->setInt("pointsLightsNumber", pointLightsNumber);
	s->setInt("directionalLightsNumber", directionalLightsNumber);
	s->setInt("spotLightsNumber", spotLightsNumber);

	s->unbind();

}

