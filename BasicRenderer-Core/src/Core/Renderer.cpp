#include "Renderer.h"

Renderer* Renderer::m_InstancePtr = nullptr;

void Renderer::setWindowTitle(std::string name) {
	m_Name = name;
	//glfwSetWindowTitle(m_Window, updatedTitle.c_str());
}
void Renderer::setSize(unsigned int w, unsigned int h) {
	m_UtilParameters.lastWidth = m_SWidth;
	m_UtilParameters.lastHeight = m_SWidth;
	m_SWidth = w; m_SHeight = h;
	glfwSetWindowSize(m_Window, w, h);
	//Resize framebuffers
	for (auto& fbo : m_Framebuffers) {
		fbo.second->resize(w, h);
	}
}

void Renderer::run() {
	cacheData();
	tick();
	terminate();
}

void Renderer::addScene(Scene* sc) {
	m_Scenes[sc->getName()] = sc;
}

Scene* Renderer::getScene(std::string sceneName) {
	return m_Scenes[sceneName];
}

void Renderer::setCurrentScene(std::string sceneName) {

	if (m_CurrentScene) {
		////////////////////
		//DECACHE ALL DATA UPLOADED TO CLIENT AND SERVER
		//////////////////////
	}

	m_CurrentScene = m_Scenes[sceneName];
};

void Renderer::renderScene() {

	if (!m_CurrentScene) {
		glClearColor(m_UtilParameters.clearColor.r,
			m_UtilParameters.clearColor.g,
			m_UtilParameters.clearColor.b,
			m_UtilParameters.clearColor.a);
		return;
	}
	//Shadow mapping pass
	if (m_CurrentScene->getLights().size() != 0)
		renderShadows();

	//Render scene in given fbo
	if (m_AntialiasingSamples > 0)
		bindFramebuffer("msaaFBO");
	else {
		if (m_PossProcess)
			bindFramebuffer("postprocessingFBO");
		else
			bindFramebuffer();
	}

	renderSceneObjects();

	if (m_AntialiasingSamples > 0) {
		if (m_PossProcess)
			//Blit msaa fbo data to vignette fbo
			blitFramebuffer("msaaFBO", "postprocessingFBO", GL_COLOR_BUFFER_BIT, GL_NEAREST);
		else
			//Blit to standard framebuffer
			blitFramebuffer("msaaFBO", GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	if (m_PossProcess) {
		possProcessPass();
	}
}

void  Renderer::setPostProcessPass(bool op) {
	if (op) {
		m_PossProcess = true;
		createVignette();
		m_Framebuffers["postprocessingFBO"] = new Framebuffer(m_Vignette->getTexture(), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);
	}
	else {
		m_PossProcess = false;

		//Delete vignette and possprocessingFBO
	}

}

void Renderer::init() {

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ------------------------------


  // glfw window creation
	m_Window = glfwCreateWindow(m_SWidth, m_SHeight, m_Name.c_str(), NULL, NULL);
	glfwSetWindowPos(m_Window, 45, 45);

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
		InputManager::onWindowResize(w, width, heigth);
	};
	auto keyFunc = [](GLFWwindow* w, int key, int scancode, int action, int mods)
	{
		InputManager::onKeyPressed(w, key, scancode, action, mods);
		UIManager::implementKeyboardCB(w, key, scancode, action, mods);
	};
	auto mouseFunc = [](GLFWwindow* w, double xpos, double ypos)
	{
		InputManager::onMouseMoved(w, xpos, ypos);
		UIManager::implementMouseCB(w, xpos, ypos);
	};


	glfwMakeContextCurrent(m_Window);

	if (m_UtilParameters.vsync)
		glfwSwapInterval(1); //V-Sync

	UIManager::initUIContext(m_Window, GLSL_VERSION);

	glfwSetFramebufferSizeCallback(m_Window, frameBufferFunc);
	glfwSetKeyCallback(m_Window, keyFunc);
	glfwSetCursorPosCallback(m_Window, mouseFunc);

	if (glewInit() != GLEW_OK) {

		std::cout << "Problem initializing glew\n";
	}


	std::cout << glGetString(GL_VERSION) << std::endl;

	//glfwSetWindowTitle(m_Window, &m_Name);
}

void Renderer::lateInit()
{
	m_PPEffects.gammaCorrection = true;
	m_PPEffects.bloom = false;

	m_UtilParameters.isFullscreen = false;
	m_UtilParameters.vsync = true;
	m_UtilParameters.secondCounter = 0;
	m_UtilParameters.fps = 0;
	m_UtilParameters.clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
	m_UtilParameters.firstMouse = true;
	m_UtilParameters.deltaTime = 0.0;
	m_UtilParameters.lastFrame = 0.0;
	m_UtilParameters.mouselastX = m_SWidth * .5f;
	m_UtilParameters.mouselastY = m_SHeight * .5f;
	m_UtilParameters.lastWidth = m_SWidth;
	m_UtilParameters.lastHeight = m_SWidth;

	std::cout << "Compiling core shaders..." << std::endl;

	m_Shaders["UnlitBasicShader"] = new Shader("UnlitBasicShader.shader", ShaderType::UNLIT);
	m_Shaders["BasicDepthShader"] = new Shader("BasicDepthShader.shader", ShaderType::UNLIT);
	m_Shaders["PointShadowDepthShader"] = new Shader("PointShadowDepthShader.shader", ShaderType::UNLIT);
	m_Shaders["BasicPhongShader"] = new Shader("BasicPhongShader.shader", ShaderType::LIT);
	m_Shaders["SkyboxShader"] = new Shader("SkyboxShader.shader", ShaderType::UNLIT);
	m_Shaders["NormalDebugShader"] = new Shader("NormalVisualizationShader.shader", ShaderType::UNLIT);
	m_Shaders["PhysicallyBasedShader"] = new Shader("PhysicallyBasedShader.shader", ShaderType::LIT);

	m_Controllers.push_back(CameraController(WASD));
	m_ActiveController = &m_Controllers[0];


}

void Renderer::cacheData() {

	if (m_CurrentScene) {
		//Set all shaders to materials and generate and bind meshes vertex buffers
		std::cout << "Generating textures and vertex buffers..." << std::endl;
		for (auto& m : m_CurrentScene->getModels()) {
			m.second->getMesh()->generateBuffers();


			for (size_t i = 0; i < m.second->getMesh()->getNumberOfMeshes(); i++)
			{
				m.second->getMaterialReference(i)->generateTextures();
				auto shaderID = m.second->getMaterialReference(i)->getShaderNameID();
				if (!m.second->getMaterialReference(i)->getShader())
					m.second->getMaterialReference(i)->setShader(m_Shaders[shaderID]);

			}
		}
		if (m_CurrentScene->getSkybox()) {
			m_CurrentScene->getSkybox()->getMaterial()->generateTextures();
			auto shaderID = m_CurrentScene->getSkybox()->getMaterial()->getShaderNameID();
			m_CurrentScene->getSkybox()->getMaterial()->setShader(m_Shaders[shaderID]);
		}

		//Upload lights to light manager
		LightManager::generateShadowMaps();

	}

	//Create and setup basic FBs
	if (m_AntialiasingSamples > 0)
		m_Framebuffers["msaaFBO"] = new Framebuffer(new Texture(m_SWidth, m_SHeight, m_AntialiasingSamples), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);

}

void Renderer::tick()
{
	while (!glfwWindowShouldClose(m_Window))
	{

		float currentFrame = glfwGetTime();
		m_UtilParameters.deltaTime = currentFrame - m_UtilParameters.lastFrame;
		m_UtilParameters.lastFrame = currentFrame;
		profile();

		renderScene();
		UIManager::update();
		UIManager::render();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
void  Renderer::terminate() {
	//Hnadle all memory erasings

	UIManager::terminateUI();
	glfwTerminate();
}

void Renderer::createVignette()
{
	m_Vignette = new Vignette(m_SWidth, m_SHeight);
}

void Renderer::renderSceneObjects()
{
	glViewport(0, 0, m_SWidth, m_SHeight);

	m_CurrentScene->getActiveCamera()->setProj(m_SWidth, m_SHeight);

	glClearColor(m_UtilParameters.clearColor.r,
		m_UtilParameters.clearColor.g,
		m_UtilParameters.clearColor.b,
		m_UtilParameters.clearColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Just in case although each material handles depth testing
	glEnable(GL_DEPTH_TEST);

	if (m_CurrentScene->getLights().size() != 0)
		renderAndCacheLights(true);

	std::vector<Model*> opaqueModels;
	std::vector<Model*> blendModels;


	for (auto& m : m_CurrentScene->getModels()) {
		m.second->getMaterialReference()->getTransparency() ? blendModels.push_back(m.second) : opaqueModels.push_back(m.second);
	}

	//FIRST = OPAQUE OBJECTS
	for (auto& m : opaqueModels) {

		renderModel(m);

	}


	//Calculate distance
	std::map<float, Model*> sorted;
	for (unsigned int i = 0; i < blendModels.size(); i++)
	{
		float distance = glm::distance(m_CurrentScene->getActiveCamera()->getPosition(), blendModels[i]->getPosition());
		sorted[distance] = blendModels[i];
	}
	//SECOND = TRANSPARENT OBJECTS SORTED FROM NEAR TO FAR
	for (std::map<float, Model*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{

		renderModel(it->second);
	}

	if (m_CurrentScene->getSkybox()) {
		renderSkybox();
	}

}

void Renderer::renderModel(Model* m) {

	if (!m->isActive()) return;

	if (m->getMesh()) {

		for (size_t i = 0; i < m->getMesh()->getNumberOfMeshes(); i++)
		{
			Shader* mShader = m->getMaterialReference(i)->getShader();

			//Check if is instanced mesh
			mShader->bind();
			if (!m->getMesh()->isInstanced())
				mShader->setBool("u_isInstanced", false);
			else
				mShader->setBool("u_isInstanced", true);

			//Load main transformation matrices
			//Check the need for this ones for material
			glm::mat4 modelView = m_CurrentScene->getActiveCamera()->getView() * m->getTransform()->getWorldMatrix();
			mShader->setMat4("u_View", m_CurrentScene->getActiveCamera()->getView());
			mShader->setMat4("u_Proj", m_CurrentScene->getActiveCamera()->getProj());
			mShader->setMat4("u_Model", m->getTransform()->getWorldMatrix());
			mShader->setMat4("u_modelView", modelView);
			mShader->setMat4("u_modelViewProj", m_CurrentScene->getActiveCamera()->getProj() * modelView);


			m->getMaterialReference(i)->setupParameters();

			m->getMaterialReference(i)->cacheUniforms();

			m->getMesh()->draw(i);

			m->getMaterialReference(i)->decacheUniforms();

			mShader->unbind();
		}
	}
	else
		std::cout << "Model " << m->getName() << " doesnt have any mesh loaded" << std::endl;

}


void Renderer::renderAndCacheLights(bool enableGizmos)
{
	for (auto& shader : m_Shaders) {
		if (shader.second->getType() == ShaderType::LIT)
			LightManager::uploadLightDataToShader(shader.second);
	}

	if (enableGizmos) {
		/*for (size_t i = 0; i < m_LightManager->getLightsCount(); i++)
		{
			Light* l = m_LightManager->getLight(i);
			m_LightManager->getDebugMaterial()->setColor(l->getColor());
			m_LightManager->getLightModel(l->getType())->setPosition(l->getPosition());
			renderModel(m_LightManager->getLightModel(l->getType()));

		}*/
	}
}

void Renderer::renderObjectNormals()
{
	Shader* normalShader = m_Shaders["NormalDebugShader"];
	normalShader->bind();

	for (auto& m : m_CurrentScene->getModels()) {
		if (!m.second->isActive()) continue;
		auto mesh = m.second->getMesh();
		if (mesh != nullptr) {
			normalShader->setMat4("u_modelView", m_CurrentScene->getActiveCamera()->getView() * m.second->getTransform()->getWorldMatrix());
			normalShader->setMat4("u_projection", m_CurrentScene->getActiveCamera()->getProj());
			mesh->draw();
		}
		else
			std::cout << "Model doesnt have any mesh loaded" << std::endl;

	}

	normalShader->unbind();
}

void Renderer::renderSkybox() {

	m_CurrentScene->getSkybox()->getMaterial()->getShader()->bind();

	m_CurrentScene->getSkybox()->getMaterial()->getShader()->setMat4("u_viewProj", m_CurrentScene->getActiveCamera()->getProj() * glm::lookAt(glm::vec3(0.0f), -m_CurrentScene->getActiveCamera()->getTransform()->getForward(), m_CurrentScene->getActiveCamera()->getTransform()->getUp()));//Remove view translation

	m_CurrentScene->getSkybox()->getMaterial()->setupParameters();

	m_CurrentScene->getSkybox()->getMaterial()->cacheUniforms();

	m_CurrentScene->getSkybox()->draw();

	m_CurrentScene->getSkybox()->getMaterial()->decacheUniforms();

	m_CurrentScene->getSkybox()->getMaterial()->getShader()->unbind();

}

void Renderer::possProcessPass() {
	//Render to texture for possprocessing
	bindFramebuffer();

	glClearColor(m_UtilParameters.clearColor.r,
		m_UtilParameters.clearColor.g,
		m_UtilParameters.clearColor.b,
		m_UtilParameters.clearColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	//Setup poss process effects
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setBool("useGammaCorrection", m_PPEffects.gammaCorrection);


	///*************

	m_Vignette->draw();
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

void Renderer::blitFramebuffer(std::string src_name, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Framebuffers[src_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLcall(glBlitFramebuffer(0, 0, src->getWidth(), src->getHeight(), 0, 0, m_SWidth, m_SHeight, mask, filter));
}


void Renderer::renderShadows()
{
	glViewport(0, 0, m_ShadowResolution, m_ShadowResolution);

	if (!m_Framebuffers["depthFBO"]) {
		Light* firstLight = m_CurrentScene->getLights().begin()->second;
		m_Framebuffers["depthFBO"] = new Framebuffer(firstLight->getShadowText(), GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_FALSE, GL_FALSE);
	}

	LightManager::renderShadows();
}

void Renderer::profile() {

	m_UtilParameters.secondCounter += m_UtilParameters.deltaTime;

	if (m_UtilParameters.secondCounter >= 1) {
		m_UtilParameters.secondCounter = 0;
		m_UtilParameters.fps = 1 / m_UtilParameters.deltaTime;
		std::string updatedTitle = m_Name + " - FPS " + std::to_string(m_UtilParameters.fps) + " / ms " + std::to_string(m_UtilParameters.deltaTime * 1000);
		glfwSetWindowTitle(m_Window, updatedTitle.c_str());
	}

}







