#include "LightManager.h"


void  LightManager::generateShadowMaps() {
	std::unordered_map<std::string, Light*> lights = Renderer::getInstance()->m_CurrentScene->getLights();
	unsigned int shadowResolution = Renderer::getInstance()->m_Settings.shadowResolution;

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	unsigned int counter{ 0 };

	for (auto& l : lights) {
		if (counter == MAX_LIGHTS) return;
		switch (l.second->getType())
		{
		case 0:
			l.second->setShadowText(new CubeMapTexture(0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE));

			break;
		case 1:
			l.second->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, shadowResolution, shadowResolution, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
			GLcall(glBindTexture(GL_TEXTURE_2D, l.second->getShadowText()->getID()));
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			GLcall(glBindTexture(GL_TEXTURE_2D, 0));
			break;
		case 2:
			l.second->setShadowText(new Texture(0, GL_DEPTH_COMPONENT16, shadowResolution, shadowResolution, 0,
				GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
			GLcall(glBindTexture(GL_TEXTURE_2D, l.second->getShadowText()->getID()));
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			GLcall(glBindTexture(GL_TEXTURE_2D, 0));
			break;
		}
		counter++;
	}


}



void LightManager::uploadLightDataToShader(Shader* s)
{
	Renderer* r = Renderer::getInstance();
	std::unordered_map<std::string, Light*> lights = r->m_CurrentScene->getLights();
	float ambientStrength = r->m_CurrentScene->getAmbientStrength();
	glm::vec3 ambientColor = r->m_CurrentScene->getAmbientColor();
	glm::mat4 view = r->m_CurrentScene->getActiveCamera()->getView();

	s->bind();

	int pointLightIndex = 0;
	int dirLightIndex = 0;
	int spotLightIndex = 0;
	int textureSlotIndex = 7;

	for (auto& lightData : lights) {

		Light* l = lightData.second;
		if (l->isActive()) {

			switch (l->getType())
			{

			case 0:
				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].pos", view * glm::vec4(l->getPosition(), 1.0));
				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].worldPos", l->getPosition());
				s->setVec3("pointLights[" + std::to_string(pointLightIndex) + "].color", l->getColor());
				s->setFloat("pointLights[" + std::to_string(pointLightIndex) + "].intensity", l->getIntensity());
				s->setFloat("pointLights[" + std::to_string(pointLightIndex) + "].influence", dynamic_cast<PointLight*>(l)->getAreaOfInfluence());
				s->setBool("pointLights[" + std::to_string(pointLightIndex) + "].castShadows", l->getCastShadows());
				if (l->getCastShadows()) {
					l->getShadowText()->bind(textureSlotIndex);
					s->setInt("pointLights[" + std::to_string(pointLightIndex) + "].shadowMap", textureSlotIndex);
					textureSlotIndex++;
				}
				pointLightIndex++;
				break;
			case 1:
				s->setVec3("directionalLights[" + std::to_string(dirLightIndex) + "].dir", view * glm::vec4(dynamic_cast<DirectionalLight*>(l)->getDirection(), 0.0));
				s->setVec3("directionalLights[" + std::to_string(dirLightIndex) + "].color", l->getColor());
				s->setFloat("directionalLights[" + std::to_string(dirLightIndex) + "].intensity", l->getIntensity());
				s->setBool("directionalLights[" + std::to_string(dirLightIndex) + "].castShadows", l->getCastShadows());
				if (l->getCastShadows()) {
					s->setMat4("directionalLights[" + std::to_string(dirLightIndex) + "].lightViewProj", l->getLightTransformMatrix());
					l->getShadowText()->bind(textureSlotIndex);
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

	s->setInt("pointsLightsNumber", pointLightIndex);
	s->setInt("directionalLightsNumber", dirLightIndex);
	s->setInt("spotLightsNumber", spotLightIndex);

	s->setFloat("u_shadowsFarPlane", r->m_Settings.shadowFarPlane);

	s->setFloat("u_ambientStrength", ambientStrength);
	s->setVec3("u_ambientColor", ambientColor);


	s->unbind();

}
void LightManager::renderShadows() {

	Renderer* r = Renderer::getInstance();

	std::unordered_map<std::string, Light*> lights = r->m_CurrentScene->getLights();
	std::unordered_map<std::string, Model*> models = r->m_CurrentScene->getModels();
	Framebuffer* depthBuffer = r->m_Resources.framebuffers["depthFBO"];
	Shader* depthShader = r->m_Resources.shaders["BasicDepthShader"];
	Shader* quadDepthShader = r->m_Resources.shaders["PointShadowDepthShader"];

	unsigned int counter{ 0 };

	for (auto& lightData : lights) {
		if (counter == MAX_LIGHTS) return;
		counter++;

		Light* l = lightData.second;

		if (l->getCastShadows()/* && (l->getTransform()->getIsDirty())*/) {


			if (l->getType() == 0) {

				glm::mat4 lightProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, r->m_Settings.shadowFarPlane);

				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(l->getPosition(), l->getPosition() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

				quadDepthShader->bind();
				for (unsigned int i = 0; i < 6; ++i)
					quadDepthShader->setMat4("cubeMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
				quadDepthShader->setVec3("lightPos", l->getPosition());
				quadDepthShader->setFloat("far_plane", r->m_Settings.shadowFarPlane);
				quadDepthShader->unbind();

				depthBuffer->setTextureAttachment(l->getShadowText(), GL_DEPTH_ATTACHMENT);

				depthBuffer->bind();

				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);


				glClear(GL_DEPTH_BUFFER_BIT);

				for (auto& m : models) {

					if (!m.second->isActive()) continue;
					if (!m.second->getMesh()->getCastShadows()) continue;

					quadDepthShader->bind();

					quadDepthShader->setMat4("u_model", m.second->getTransform()->getWorldMatrix());

					if (!m.second->getMesh()->isInstanced())
						quadDepthShader->setBool("u_isInstanced", false);
					else
						quadDepthShader->setBool("u_isInstanced", true);

					for (size_t k = 0; k < m.second->getMesh()->getNumberOfMeshes(); k++)
					{
						if (m.second->getMesh()->getMeshBuffertData(k).numFaces != 2)
							glCullFace(GL_FRONT);
						else
							glCullFace(GL_BACK);

						m.second->getMesh()->draw(k);
					}

					quadDepthShader->unbind();

				}

			}
			else {


				depthBuffer->setTextureAttachment(l->getShadowText(), GL_TEXTURE_2D);

				depthBuffer->bind();

				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);

				glClear(GL_DEPTH_BUFFER_BIT);

				for (auto& m : models) {

					if (!m.second->isActive()) continue;
					if (!m.second->getMesh()->getCastShadows()) continue;

					depthShader->bind();


					if (!m.second->getMesh()->isInstanced()) {
						depthShader->setBool("u_isInstanced", false);
						depthShader->setMat4("u_Light_ModelViewProj", l->getLightTransformMatrix() * m.second->getTransform()->getWorldMatrix());
						depthShader->setMat4("u_model", m.second->getTransform()->getWorldMatrix());
					}
					else {
						depthShader->setBool("u_isInstanced", true);
						depthShader->setMat4("u_viewProj", l->getLightTransformMatrix());
					}

					for (size_t k = 0; k < m.second->getMesh()->getNumberOfMeshes(); k++)
					{
						if (m.second->getMesh()->getMeshBuffertData(k).numFaces != 2)
							glCullFace(GL_FRONT);
						else
							glCullFace(GL_BACK);
						m.second->getMesh()->draw(k);
					}

					depthShader->unbind();

				}
			}
			l->getTransform()->setIsDirty(false);
		}

	}
}