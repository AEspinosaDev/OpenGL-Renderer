#include "Renderer.h"
#include <Core/Lights/PointLight.h>
#include <Core/Lights/DirectionalLight.h>
#include <Core/Materials/BasicPhongMaterial.h>
#include <map>

void Renderer::run() {
	setupScene();
	tick();
	glfwTerminate();
}

void Renderer::createScene(std::string sceneName) {
	m_Scenes[sceneName] = new Scene(sceneName);
}

Scene* Renderer::getScene(std::string sceneName) {
	return m_Scenes[sceneName];
}

void Renderer::setupScene() {

	/*BasicPhongMaterial* sphere_m = new BasicPhongMaterial(m_Shaders);
	sphere_m->setShininess(500);
	sphere_m->setSpecularity(3);
	Model* sphere = new Model();
	sphere->loadMesh("box.obj");
	sphere->loadMaterial(sphere_m);
	sphere->setPosition(glm::vec3(-2.0, 2.0, 0.0));
	m_Models["sphere"] = sphere;*/

	

	m_LightManager->setAmbientStrength(0.1);
	m_LightManager->setAmbientColor(glm::vec3(0.2, 0.2, 1.0));

	if (m_CurrentScene) {
		auto lights = m_CurrentScene->getLights();

		for (auto& l : lights) {
			m_LightManager->addLight(l.second);
		}
	}
	

	createVignette();

	if (m_AntialiasingSamples > 0)
		m_Framebuffers["msaaFBO"] = new Framebuffer(new Texture(m_SWidth, m_SHeight, m_AntialiasingSamples), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);
	
	//m_Framebuffers["depthFBO"] = new Framebuffer(m_LightManager->getLight(0)->getShadowText(), GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_FALSE, GL_FALSE);
	m_Framebuffers["postprocessingFBO"] = new Framebuffer(m_Vignette->getTexture(), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);


}
void Renderer::renderScene() {

	if (!m_CurrentScene) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		return;
	}

	//Shadow mapping pass
	if (m_LightManager->getLightsCount() != 0)
		computeShadows();


	//Render scene in given fbo
	if (m_AntialiasingSamples > 0)
		bindFramebuffer("msaaFBO");
	else {
		bindFramebuffer("postprocessingFBO");
	}

	m_CurrentScene->getActiveCamera()->setProj(45.0f, m_SWidth, m_SHeight);

	glViewport(0, 0, m_SWidth, m_SHeight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


	glEnable(GL_DEPTH_TEST);


	renderModelMeshes();

	//debugObjectNormals();


	if (m_AntialiasingSamples > 0) {
		//Blit msaa fbo data to vignette fbo
		blitFramebuffer("msaaFBO", "postprocessingFBO", GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}


	//Render to texture for possprocessing
	bindFramebuffer();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	/*glClear(GL_COLOR_BUFFER_BIT);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	m_Vignette->draw();

}

void Renderer::init() {
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ------------------------------


  // glfw window creation
	m_Window = glfwCreateWindow(m_SWidth, m_SHeight, &m_Name, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	// --------------------

	glfwSetWindowUserPointer(m_Window, this);

	auto frameBufferFunc = [](GLFWwindow* w, int width, int heigth)
	{
		static_cast<Renderer*>(glfwGetWindowUserPointer(w))->FramebufferResize_Callback(w, width, heigth);
	};
	auto keyFunc = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		static_cast<Renderer*>(glfwGetWindowUserPointer(w))->Key_Callback(w, key, scancode, action, mods);
	};
	auto mouseFunc = [](GLFWwindow* w, double xpos, double ypos)
	{
		static_cast<Renderer*>(glfwGetWindowUserPointer(w))->Mouse_Callback(w, xpos, ypos);
	};


	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, frameBufferFunc);
	glfwSetKeyCallback(m_Window, keyFunc);
	glfwSetCursorPosCallback(m_Window, mouseFunc);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glewInit() != GLEW_OK) {

		std::cout << "Problem initializing glew\n";
	}




	std::cout << glGetString(GL_VERSION) << std::endl;

}

void Renderer::lateInit()
{
	std::cout << "Compiling shaders..." << std::endl;

	m_Shaders["UnlitBasicShader"] = new Shader("UnlitBasicShader.shader", ShaderType::UNLIT);
	m_Shaders["BasicDepthShader"] = new Shader("BasicDepthShader.shader", ShaderType::UNLIT);
	m_Shaders["PointShadowDepthShader"] = new Shader("PointShadowDepthShader.shader", ShaderType::UNLIT);
	m_Shaders["BasicPhongShader"] = new Shader("BasicPhongShader.shader", ShaderType::LIT);
	m_Shaders["SkyboxShader"] = new Shader("SkyboxShader.shader", ShaderType::UNLIT);
	m_Shaders["NormalDebugShader"] = new Shader("NormalVisualizationShader.shader", ShaderType::UNLIT);

	m_LightManager->init(m_Shaders);
}

void Renderer::tick()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
		renderScene();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Renderer::Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	m_CurrentScene->getActiveCamera()->camMovement(window, m_DeltaTime);

	//WIP LIGHT CONTROLS
	Light* l = m_LightManager->getLight(0);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x - .5f, l->getPosition().y, l->getPosition().z));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x + .5f, l->getPosition().y, l->getPosition().z));
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x, l->getPosition().y + .5f, l->getPosition().z));
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x, l->getPosition().y - .5f, l->getPosition().z));

}

void Renderer::Mouse_Callback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		if (m_FirstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_FirstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos; // reversed since y-coordinates go from bottom to top

		m_lastX = xpos;
		m_lastY = ypos;

		m_CurrentScene->getActiveCamera()->camRotation(xoffset, yoffset);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
		m_FirstMouse = true;
	}

}

void Renderer::FramebufferResize_Callback(GLFWwindow* window, int width, int height)
{

	m_SWidth = width;
	m_SHeight = height;

	//Resize framebuffers
	for (auto& fbo : m_Framebuffers) {
		fbo.second->resize(width, height);
	}

}



void Renderer::createVignette()
{
	m_Vignette = new Vignette(m_SWidth, m_SHeight);
}

void Renderer::renderModelMeshes()
{
	if (m_LightManager->getLightsCount() != 0)
		renderLights(true);

	std::vector<Model*> opaqueModels;
	std::vector<Model*> blendModels;

	/*m_Shaders["BasicPhongShader"]->bind();
	m_Shaders["BasicPhongShader"]->setInt("u_skybox", 5);
	m_Skybox->getMaterial()->getTexture()->bind(5);
	m_Shaders["BasicPhongShader"]->unbind();*/


	for (auto& m : m_CurrentScene->getModels()) {
		m.second->getMesh()->getMaterial()->getTransparency() ? blendModels.push_back(m.second) : opaqueModels.push_back(m.second);
	}

	//FIRST = OPAQUE OBJECTS
	for (auto& m : opaqueModels) {
		m->draw(m_CurrentScene->getActiveCamera()->getProj(), m_CurrentScene->getActiveCamera()->getView());
	}

	if (blendModels.size() == 0) return;

	//Calculate distance
	std::map<float, Model*> sorted;
	for (unsigned int i = 0; i < blendModels.size(); i++)
	{
		float distance = glm::distance(m_CurrentScene->getActiveCamera()->getPos(), blendModels[i]->getPosition());
		sorted[distance] = blendModels[i];
	}
	//SECOND = TRANSPARENT OBJECTS SORTED FROM NEAR TO FAR
	for (std::map<float, Model*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		it->second->draw( m_CurrentScene->getActiveCamera()->getProj(), m_CurrentScene->getActiveCamera()->getView());
	}

	if (m_CurrentScene->getSkybox())
		renderSkybox();

}

void Renderer::renderObject(std::string name) {

	/*Model* m = m_Models[name];
	m->draw(m_CurrentScene->getActiveCamera()->getProj(), m_CurrentScene->getActiveCamera()->getView());*/

}


void Renderer::renderLights(bool enableGizmos)
{
	for (auto& shader : m_Shaders) {
		if (shader.second->getType() == ShaderType::LIT)
			m_LightManager->uploadLightDataToShader(shader.second, m_CurrentScene->getActiveCamera()->getView());
	}
	if (enableGizmos)
		m_LightManager->drawLights(m_CurrentScene->getActiveCamera()->getProj(), m_CurrentScene->getActiveCamera()->getView());

}

void Renderer::renderObjectNormals()
{
	Shader* normalShader = m_Shaders["NormalDebugShader"];
	normalShader->bind();

	for (auto& m : m_CurrentScene->getModels()) {
		if (!m.second->isActive()) continue;
		auto mesh = m.second->getMesh();
		if (mesh != nullptr) {
			mesh->setModel(m.second->getTransform());
			normalShader->setMat4("u_modelView", m_CurrentScene->getActiveCamera()->getView() * m.second->getMesh()->getModel());
			normalShader->setMat4("u_projection", m_CurrentScene->getActiveCamera()->getProj());
			mesh->draw();
		}
		else
			std::cout << "Model doesnt have any mesh loaded" << std::endl;



	}

	normalShader->unbind();
}

void Renderer::renderSkybox() {

	m_CurrentScene->getSkybox()->draw(m_CurrentScene->getActiveCamera()->getProj(), glm::lookAt(glm::vec3(0.0f), m_CurrentScene->getActiveCamera()->getFront(), m_CurrentScene->getActiveCamera()->getUp())); //Remove view translation
}

void Renderer::bindFramebuffer() {
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::bindFramebuffer(std::string name) {
	m_Framebuffers[name]->bind();

}
void Renderer::blitFramebuffer(std::string src_name, std::string dst_name, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Framebuffers[src_name];
	Framebuffer* dst = m_Framebuffers[dst_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getID()));
	GLcall(glBlitFramebuffer(0, 0, src->getWidth(), src->getHeight(), 0, 0, dst->getWidth(), dst->getHeight(), mask, filter));
}

void Renderer::blitFramebuffer(std::string src_name, unsigned int src_x_o, unsigned int src_y_o, unsigned int src_x_f, unsigned int src_y_f,
	std::string dst_name, unsigned int dst_x_o, unsigned int dst_y_o, unsigned int dst_x_f, unsigned int dst_y_f, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Framebuffers[src_name];
	Framebuffer* dst = m_Framebuffers[dst_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getID()));
	GLcall(glBlitFramebuffer(src_x_o, src_y_o, src_x_f, src_y_f, dst_x_o, dst_y_o, dst_x_f, dst_y_f, mask, filter));
}



void Renderer::computeShadows()
{
	glViewport(0, 0, m_ShadowResolution, m_ShadowResolution);

	int lights = m_LightManager->getLightsCount();

	for (size_t i = 0; i < lights; i++)
	{
		if (!m_Framebuffers["depthFBO"])
			m_Framebuffers["depthFBO"] = new Framebuffer(m_LightManager->getLight(i)->getShadowText(), GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_FALSE, GL_FALSE);

		if (m_LightManager->getLight(i)->getCastShadows()) {


			if (m_LightManager->getLight(i)->getType() == 0) {

				glm::mat4 lightProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, m_LightManager->getShadowsFarPlane());
				
				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				shadowTransforms.push_back(lightProj *
					glm::lookAt(m_LightManager->getLight(i)->getPosition(), m_LightManager->getLight(i)->getPosition() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

				m_Shaders["PointShadowDepthShader"]->bind();
				for (unsigned int i = 0; i < 6; ++i)
					m_Shaders["PointShadowDepthShader"]->setMat4("cubeMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
				m_Shaders["PointShadowDepthShader"]->setVec3("lightPos", m_LightManager->getLight(i)->getPosition());
				m_Shaders["PointShadowDepthShader"]->setFloat("far_plane", m_LightManager->getShadowsFarPlane());
				m_Shaders["PointShadowDepthShader"]->unbind();

				


				m_Framebuffers["depthFBO"]->setTextureAttachment(m_LightManager->getLight(i)->getShadowText(), GL_DEPTH_ATTACHMENT);

				bindFramebuffer("depthFBO");

				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				glClear(GL_DEPTH_BUFFER_BIT);

				for (auto& m : m_CurrentScene->getModels()) {

					if (!m.second->isActive()) return;
					if (!m.second->getMesh()->getCastShadows()) return;

					m.second->getMesh()->setModel(m.second->getTransform());

					m_Shaders["PointShadowDepthShader"]->bind();

					m_Shaders["PointShadowDepthShader"]->setMat4("u_model",m.second->getMesh()->getModel());

					m.second->getMesh()->draw();

					m_Shaders["PointShadowDepthShader"]->unbind();

				}

			}
			else {

			

				m_Framebuffers["depthFBO"]->setTextureAttachment(m_LightManager->getLight(i)->getShadowText(), GL_TEXTURE_2D);

				bindFramebuffer("depthFBO");

				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				glClear(GL_DEPTH_BUFFER_BIT);

				for (auto& m : m_CurrentScene->getModels()) {

					if (!m.second->isActive()) return;
					if (!m.second->getMesh()->getCastShadows()) return;

					m.second->getMesh()->setModel(m.second->getTransform());

					m_Shaders["PointShadowDepthShader"]->bind();

					m_Shaders["PointShadowDepthShader"]->setMat4("u_Light_ModelViewProj", m_LightManager->getLight(i)->getLightTransformMatrix() * m.second->getMesh()->getModel());

					m.second->getMesh()->draw();

					m_Shaders["PointShadowDepthShader"]->unbind();
					
				}
			}
		}

	}


}








