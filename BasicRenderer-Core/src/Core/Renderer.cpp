#include "Renderer.h"

Renderer* Renderer::m_InstancePtr = nullptr;

void Renderer::setWindowTitle(std::string name) {
	m_Name = name;
	//glfwSetWindowTitle(m_Window, updatedTitle.c_str());
}
void Renderer::setSize(unsigned int w, unsigned int h) {
	m_UtilParams.lastWidth = m_SWidth;
	m_UtilParams.lastHeight = m_SWidth;
	m_SWidth = w; m_SHeight = h;
	glfwSetWindowSize(m_Window, w, h);
	//Resize framebuffers
	for (auto& fbo : m_Resources.framebuffers) {
		fbo.second->resize(w, h);
	}
}

void Renderer::run() {
	cacheData();
	tick();
	terminate();
}

void Renderer::addScene(Scene* sc) {
	m_Resources.scenes[sc->getName()] = sc;
}

Scene* Renderer::getScene(std::string sceneName) {
	return m_Resources.scenes[sceneName];
}

void Renderer::setCurrentScene(std::string sceneName) {

	if (m_CurrentScene) {
		////////////////////
		//DECACHE ALL DATA UPLOADED TO CLIENT AND SERVER
		//////////////////////
	}

	m_CurrentScene = m_Resources.scenes[sceneName];
};

void Renderer::renderScene() {

	if (!m_CurrentScene) {
		glClearColor(m_Settings.clearColor.r,
			m_Settings.clearColor.g,
			m_Settings.clearColor.b,
			m_Settings.clearColor.a);
		return;
	}
	for (auto& fbo : m_Resources.framebuffers) {
		fbo.second->resize(m_RWidth, m_RHeight);
	}

	//Shadow mapping pass
	if (m_CurrentScene->getLights().size() != 0)
		renderShadows();

	//Render scene in given fbo
	std::string defaultFBO;
	if (m_Settings.antialiasingSamples > 0)
		defaultFBO = "msaaFBO";
	else {
		if (m_Settings.postProcess)
			defaultFBO = "postprocessingFBO";
		else
			defaultFBO = "viewportFBO";
	}

	bindFramebuffer(defaultFBO);
	renderSceneObjects();

	if (UIManager::m_SelectedObject) highlightPass(UIManager::m_SelectedObject, defaultFBO);

	if (m_UtilParams.renderNormals || m_UtilParams.renderTangents)
		renderObjectNormals();


	if (m_Settings.antialiasingSamples > 0) {
		if (m_Settings.postProcess)
			//Blit msaa fbo data to vignette fbo
			blitFramebuffer("msaaFBO", "postprocessingFBO", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//blitFramebuffer("msaaFBO", "postprocessingFBO", GL_COLOR_BUFFER_BIT , GL_NEAREST);


		else
			//Blit to standard framebuffer
			m_Settings.editMode ? blitFramebuffer("msaaFBO", "viewportFBO", GL_COLOR_BUFFER_BIT, GL_NEAREST) : blitFramebuffer("msaaFBO", GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}



	if (m_Settings.postProcess) {
		//m_Vignette->setDepthTexture(m_Resources.framebuffers["msaaFBO"]->getDepthTextureAttachment());
		possProcessPass();
		/*if (m_Settings.enableGizmos)
			renderBillboards();*/
	}
}

void  Renderer::setPostProcessPass(bool op) {
	if (op) {
		m_Settings.postProcess = true;
		createVignette();
		Texture* depthTexture = new Texture(0, GL_DEPTH24_STENCIL8, m_RWidth, m_RHeight, 0,GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
		depthTexture->generateTexture();
		/*m_Resources.framebuffers["postprocessingFBO"] = new Framebuffer(m_Vignette->getTexture(), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);*/
		m_Resources.framebuffers["postprocessingFBO"] = new Framebuffer(m_Vignette->getTexture(), depthTexture, true);
		m_Vignette->setDepthTexture(depthTexture);
		//m_Vignette->setDepthTexture(m_Resources.framebuffers["msaaFBO"]->getDepthTextureAttachment());

	}
	else {
		m_Settings.postProcess = false;

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
	/*const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_SWidth = mode->width;
	m_SHeight = mode->height;
	m_Window = glfwCreateWindow(m_SWidth, m_SHeight, m_Name.c_str(), glfwGetPrimaryMonitor(), NULL);
	m_UtilParameters.isFullscreen = true;*/
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

	if (m_Settings.vsync)
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

	m_UtilParams.mouselastX = m_SWidth * .5f;
	m_UtilParams.mouselastY = m_SHeight * .5f;
	m_UtilParams.lastWidth = m_SWidth;
	m_UtilParams.lastHeight = m_SWidth;

	std::cout << "Compiling core shaders..." << std::endl;

	m_Resources.shaders["UnlitBasicShader"] = new Shader("UnlitBasicShader.shader", ShaderType::UNLIT);
	m_Resources.shaders["BasicDepthShader"] = new Shader("BasicDepthShader.shader", ShaderType::UNLIT);
	m_Resources.shaders["DilationShader"] = new Shader("DilationShader.shader", ShaderType::UNLIT);
	m_Resources.shaders["PointShadowDepthShader"] = new Shader("PointShadowDepthShader.shader", ShaderType::UNLIT);
	m_Resources.shaders["NormalDebugShader"] = new Shader("NormalVisualizationShader.shader", ShaderType::UNLIT);
	m_Resources.shaders["SkyboxShader"] = new Shader("SkyboxShader.shader", ShaderType::UNLIT);

	m_Resources.shaders["BasicPhongShader"] = new Shader("BasicPhongShader.shader", ShaderType::LIT);
	m_Resources.shaders["PhysicallyBasedShader"] = new Shader("PhysicallyBasedShader.shader", ShaderType::LIT);

	//Create and setup basic FBs
	//if (m_Settings.antialiasingSamples > 0)
	/*Texture* depthTexture = new Texture(0, GL_DEPTH24_STENCIL8, m_RWidth, m_RHeight,32, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, false, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	m_Resources.framebuffers["msaaFBO"] = new Framebuffer(new Texture(m_RWidth, m_RHeight,32),depthTexture,true );*/
	m_Resources.framebuffers["msaaFBO"] = new Framebuffer(new Texture(m_RWidth, m_RHeight, m_Settings.antialiasingSamples), GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);


	Texture* highLightT = new Texture(0, GL_RGBA8, m_RWidth, m_RHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, false, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	Texture* dilationT = new Texture(0, GL_RGBA8, m_RWidth, m_RHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, false, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	highLightT->generateTexture(); dilationT->generateTexture();
	m_Resources.framebuffers["highLightFBO"] = new Framebuffer(highLightT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);
	m_Resources.framebuffers["dilationFBO"] = new Framebuffer(dilationT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);
	Texture* viewPortT = new Texture(0, GL_RGBA8, m_RWidth, m_RHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, false, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	viewPortT->generateTexture();
	m_Resources.framebuffers["viewportFBO"] = new Framebuffer(viewPortT, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_TRUE, GL_TRUE);

	m_Resources.primitives["quad"] = new Quad();
	m_Resources.primitives["quad"]->generateBuffers();
	m_Resources.models["billboard"] = new Model(m_Resources.primitives["quad"], new UnlitBasicMaterial());
	auto shaderID = m_Resources.models["billboard"]->getMaterialReference(0)->getShaderNameID();
	m_Resources.models["billboard"]->getMaterialReference(0)->setFaceVisibility(BOTH);
	m_Resources.models["billboard"]->getMaterialReference(0)->setTransparency(true);
	m_Resources.models["billboard"]->getMaterialReference(0)->setAlphaTest(true);
	m_Resources.models["billboard"]->getMaterialReference(0)->setShader(m_Resources.shaders[shaderID]);
	Texture* lightText = new Texture("bulb.png");
	lightText->generateTexture();
	m_Resources.textures["lightIcon"] = lightText;
	Texture* camText = new Texture("camera.png");
	camText->generateTexture();
	m_Resources.textures["cameraIcon"] = camText;

	m_Resources.controllers.push_back(CameraController(WASD));
	m_ActiveController = &m_Resources.controllers[0];


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
					m.second->getMaterialReference(i)->setShader(m_Resources.shaders[shaderID]);

			}
		}
		if (m_CurrentScene->getSkybox()) {
			m_CurrentScene->getSkybox()->getMaterial()->generateTextures();
			auto shaderID = m_CurrentScene->getSkybox()->getMaterial()->getShaderNameID();
			m_CurrentScene->getSkybox()->getMaterial()->setShader(m_Resources.shaders[shaderID]);
		}

		//Upload lights to light manager
		LightManager::generateShadowMaps();

	}
	std::cout << "Ready." << std::endl;

}

void Renderer::tick()
{
	while (!glfwWindowShouldClose(m_Window))
	{

		float currentFrame = glfwGetTime();
		m_UtilParams.deltaTime = currentFrame - m_UtilParams.lastFrame;
		m_UtilParams.lastFrame = currentFrame;
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
	m_Vignette = new Vignette(m_RWidth, m_RHeight);
}

void Renderer::renderSceneObjects()
{

	glClearColor(m_Settings.clearColor.r,
		m_Settings.clearColor.g,
		m_Settings.clearColor.b,
		m_Settings.clearColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	glViewport(0, 0, m_RWidth, m_RHeight);

	if (!m_CurrentScene->getActiveCamera()->isActive())return;
	m_CurrentScene->getActiveCamera()->setProj(m_RWidth, m_RHeight);

	if (m_CurrentScene->getLights().size() != 0)
		cacheLights();

	std::vector<Model*> opaqueModels;
	std::vector<Model*> blendModels;


	for (auto& m : m_CurrentScene->getModels()) {
		m.second->getMaterialReference()->getTransparency() ? blendModels.push_back(m.second) : opaqueModels.push_back(m.second);
	}

	//FIRST = OPAQUE OBJECTS
	for (auto& m : opaqueModels) {
		//if (!m->isSelected())
		renderModel(m);

	}

	if (m_CurrentScene->getSkybox()) {
		renderSkybox();
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
		//if (!it->second->isSelected())
		renderModel(it->second);
	}

	if (m_Settings.enableGizmos)
		renderBillboards();


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

void Renderer::highlightPass(SceneObject* obj, const std::string defaultFBO) {
	Model* m = nullptr;
	if (!obj->isActive()) return;
	if (obj->getObjectType() == MODEL) {
		m = dynamic_cast<Model*>(obj);
	}
	else {
		m = m_Resources.models["billboard"];

		m->setPosition(obj->getPosition());
	}
	// --------------------------------------------------------------------
	bindFramebuffer("highLightFBO");
	glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	Shader* mShader = m_Resources.shaders["UnlitBasicShader"];

	for (size_t i = 0; i < m->getMesh()->getNumberOfMeshes(); i++)
	{
		mShader->bind();
		if (!m->getMesh()->isInstanced())
			mShader->setBool("u_isInstanced", false);
		else
			mShader->setBool("u_isInstanced", true);

		glm::mat4 modelView;
		if (obj->getObjectType() == MODEL)
			modelView = m_CurrentScene->getActiveCamera()->getView() * m->getTransform()->getWorldMatrix();
		else {
			mShader->setBool("u_highlightPass", true);
			modelView = m_CurrentScene->getActiveCamera()->getView() * glm::inverse(glm::lookAt(obj->getPosition(), m_CurrentScene->getActiveCamera()->getPosition(), glm::vec3(0, 1, 0)));
			mShader->setInt("u_colorTex", 0);
			if (obj->getObjectType() == LIGHT)
				m_Resources.textures["lightIcon"]->bind(0);
			else
				m_Resources.textures["cameraIcon"]->bind(0);
		}
		mShader->setMat4("u_View", m_CurrentScene->getActiveCamera()->getView());
		mShader->setMat4("u_Proj", m_CurrentScene->getActiveCamera()->getProj());
		mShader->setMat4("u_model", m->getTransform()->getWorldMatrix());
		mShader->setMat4("u_modelView", modelView);
		mShader->setMat4("u_modelViewProj", m_CurrentScene->getActiveCamera()->getProj() * modelView);
		mShader->setVec3("u_color", glm::vec3(0.0, 0.0, 1.0));
		mShader->setBool("opacity", 1.0);

		m->getMesh()->draw(i);
		mShader->setBool("u_highlightPass", false);
		mShader->unbind();
	}
	// --------------------------------------------------------------------

	bindFramebuffer("dilationFBO");
	glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setBool("dilationPass", true);
	m_Vignette->getShader()->setFloat("radius", 2.5f);
	m_Vignette->getShader()->setFloat("gridX", 1.F / (float)m_RWidth);
	m_Vignette->getShader()->setFloat("gridY", 1.F / (float)m_RHeight);

	Texture* originalT = m_Vignette->getTexture();
	m_Vignette->setTexture(m_Resources.framebuffers["highLightFBO"]->getTextureAttachment());
	m_Vignette->draw();

	glClear(GL_DEPTH_BUFFER_BIT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF); //Always pass if reference equals to 1
	glStencilMask(0xFF); //Enable stencil writing
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setBool("contourPass", true);

	m_Vignette->getShader()->setInt("contourTex", 1);
	m_Resources.framebuffers["dilationFBO"]->getTextureAttachment()->bind(1);
	m_Vignette->draw();
	m_Vignette->getShader()->bind();
	m_Resources.framebuffers["dilationFBO"]->getTextureAttachment()->unbind();
	m_Vignette->getShader()->setBool("contourPass", false);
	m_Vignette->getShader()->unbind();

	glStencilFunc(GL_ALWAYS, 0, 0xFF); //Always pass if reference equals to 0

	//// copy stencil buffer from target B
	blitFramebuffer("dilationFBO", 0, 0, m_RWidth, m_RHeight, defaultFBO, 0, 0, m_RWidth, m_RHeight, GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	//// bind to main frame buffer
	bindFramebuffer(defaultFBO);


	//// blit target B to the default frame buffer
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00); //Not write

	glDepthMask(false);
	glDisable(GL_DEPTH_TEST);
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setBool("dilationPass", false);
	m_Vignette->getShader()->setBool("useGammaCorrection", false);
	m_Vignette->getShader()->setBool("useFog", false);
	m_Vignette->setTexture(m_Resources.framebuffers["dilationFBO"]->getTextureAttachment());

	m_Vignette->draw();
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setBool("useGammaCorrection", m_Settings.ppEffects.gammaCorrection);
	m_Vignette->setTexture(originalT);
	m_Vignette->getShader()->unbind();
	//// disable stencil buffer
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	//// --------------------------------------------------------------------
	//renderModel(m);




}
void Renderer::cacheLights()
{
	for (auto& shader : m_Resources.shaders) {
		if (shader.second->getType() == ShaderType::LIT)
			LightManager::uploadLightDataToShader(shader.second);
	}


}
void Renderer::renderBillboards() {
	Model* billboard = m_Resources.models["billboard"];

	UnlitBasicMaterial* billboardMat = static_cast<UnlitBasicMaterial*>(billboard->getMaterialReference());
	Shader* mShader = billboardMat->getShader();
	for (auto& obj : m_CurrentScene->getSceneObjects())
	{
		if (obj.second->getObjectType() == MODEL) continue;
		if (obj.second->getObjectType() == LIGHT) {

			billboardMat->addColorTex(m_Resources.textures["lightIcon"]);
			/*billboardMat->setOverrideColor(true);
			billboardMat->setColor()*/
		}
		else
		{
			billboardMat->addColorTex(m_Resources.textures["cameraIcon"]);
		}

		billboard->setPosition(obj.second->getPosition());
		glm::vec3 pos = billboard->getPosition();
		glm::vec3 camPos = m_CurrentScene->getActiveCamera()->getPosition();
		Model* m = billboard;
		mShader->bind();
		mShader->setBool("u_isInstanced", false);

		glm::mat4 modelView = m_CurrentScene->getActiveCamera()->getView() * glm::inverse(glm::lookAt(pos, camPos, glm::vec3(0, 1, 0)));
		mShader->setMat4("u_View", m_CurrentScene->getActiveCamera()->getView());
		mShader->setMat4("u_Proj", m_CurrentScene->getActiveCamera()->getProj());
		mShader->setMat4("u_modelView", modelView);
		mShader->setMat4("u_modelViewProj", m_CurrentScene->getActiveCamera()->getProj() * modelView);


		m->getMaterialReference()->setupParameters();

		m->getMaterialReference()->cacheUniforms();

		m->getMesh()->draw();

		m->getMaterialReference()->decacheUniforms();

		mShader->unbind();
	}


}

void Renderer::renderObjectNormals()
{
	Shader* normalShader = m_Resources.shaders["NormalDebugShader"];
	normalShader->bind();

	for (auto& m : m_CurrentScene->getModels()) {
		if (!m.second->isActive()) continue;
		auto mesh = m.second->getMesh();
		if (mesh != nullptr) {
			normalShader->setMat4("u_modelView", m_CurrentScene->getActiveCamera()->getView() * m.second->getTransform()->getWorldMatrix());
			normalShader->setMat4("u_projection", m_CurrentScene->getActiveCamera()->getProj());
			normalShader->setBool("u_isInstanced", m.second->getMesh()->isInstanced());

			if (m_UtilParams.renderNormals) {
				normalShader->setBool("u_renderNormal", true);
				normalShader->setBool("u_renderTangent", false);
				mesh->draw();
			}

			if (m_UtilParams.renderTangents) {
				normalShader->setBool("u_renderNormal", false);
				normalShader->setBool("u_renderTangent", true);
				mesh->draw();
			}
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
	m_Settings.editMode ? bindFramebuffer("viewportFBO") : bindFramebuffer();

	glClearColor(m_Settings.clearColor.r,
		m_Settings.clearColor.g,
		m_Settings.clearColor.b,
		m_Settings.clearColor.a);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	//Setup poss process effects
	m_Vignette->getShader()->bind();
	m_Vignette->getShader()->setFloat("far", m_CurrentScene->getActiveCamera()->getFar());
	m_Vignette->getShader()->setFloat("near", m_CurrentScene->getActiveCamera()->getNear());

	m_Vignette->getShader()->setBool("useGammaCorrection", m_Settings.ppEffects.gammaCorrection);
	m_Vignette->getShader()->setBool("useFog", m_Settings.ppEffects.fog);
	m_Vignette->getShader()->setInt("fogType", (int)m_Settings.ppEffects.fogType);
	m_Vignette->getShader()->setVec3("fogColor", m_Settings.ppEffects.fogColor);

	m_Vignette->getShader()->setFloat("fogIntensity", m_Settings.ppEffects.fogIntensity);
	m_Vignette->getShader()->setFloat("fogEnd", m_Settings.ppEffects.fogEnd);
	m_Vignette->getShader()->setFloat("fogStart", m_Settings.ppEffects.fogStart);



	///*************

	m_Vignette->draw();
}

void Renderer::bindFramebuffer() {
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::bindFramebuffer(std::string name) {
	m_Resources.framebuffers[name]->bind();

}
void Renderer::blitFramebuffer(std::string src_name, std::string dst_name, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Resources.framebuffers[src_name];
	Framebuffer* dst = m_Resources.framebuffers[dst_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getID()));
	GLcall(glBlitFramebuffer(0, 0, src->getWidth(), src->getHeight(), 0, 0, dst->getWidth(), dst->getHeight(), mask, filter));
}

void Renderer::blitFramebuffer(std::string src_name, unsigned int src_x_o, unsigned int src_y_o, unsigned int src_x_f, unsigned int src_y_f,
	std::string dst_name, unsigned int dst_x_o, unsigned int dst_y_o, unsigned int dst_x_f, unsigned int dst_y_f, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Resources.framebuffers[src_name];
	Framebuffer* dst = m_Resources.framebuffers[dst_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getID()));
	GLcall(glBlitFramebuffer(src_x_o, src_y_o, src_x_f, src_y_f, dst_x_o, dst_y_o, dst_x_f, dst_y_f, mask, filter));
}

void Renderer::blitFramebuffer(std::string src_name, GLbitfield mask, GLenum filter) {

	Framebuffer* src = m_Resources.framebuffers[src_name];

	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getID()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
	GLcall(glBlitFramebuffer(0, 0, src->getWidth(), src->getHeight(), 0, 0, m_RWidth, m_RHeight, mask, filter));
}


void Renderer::renderShadows()
{
	glViewport(0, 0, m_Settings.shadowResolution, m_Settings.shadowResolution);

	if (!m_Resources.framebuffers["depthFBO"]) {
		Light* firstLight = m_CurrentScene->getLights().begin()->second;
		m_Resources.framebuffers["depthFBO"] = new Framebuffer(firstLight->getShadowText(), GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_FALSE, GL_FALSE);
	}

	LightManager::renderShadows();
}

void Renderer::profile() {

	m_UtilParams.secondCounter += m_UtilParams.deltaTime;

	if (m_UtilParams.secondCounter >= 1) {
		m_UtilParams.secondCounter = 0;
		m_UtilParams.fps = 1 / m_UtilParams.deltaTime;
		std::string updatedTitle = m_Name + " - FPS " + std::to_string(m_UtilParams.fps) + " / ms " + std::to_string(m_UtilParams.deltaTime * 1000);
		glfwSetWindowTitle(m_Window, updatedTitle.c_str());
	}

}

void  Renderer::clear(bool color, bool depth, bool stencil) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}







