#include "Renderer.h"

void Renderer::Run() {
	Init();
	LateInit();
	SetupScene();
	Tick();
	glfwTerminate();
}

void Renderer::SetupScene() {
	Shader* mainShader = new Shader("BasicPhongShader.shader");
	m_Shaders["basicShader"] = mainShader;


	Texture* boxColorTex = new Texture("SeamlessWood-Diffuse.jpg");
	Texture* boxNormalTex = new Texture("SeamlessWood-NormalMap.tif");
	Material* box_m = new Material(mainShader);
	box_m->addColorTex(boxColorTex);
	box_m->addNormalTex(boxNormalTex);

	Model* box = new Model();
	box->loadMesh("box.obj");
	box->loadMaterial(box_m);
	box->setPosition(glm::vec3(2.0, 0.5, 0.0));
	m_Models["box"] = box;

	Texture* floorAlbedoTex = new Texture("floor.jpg");
	Texture* floorNormalTex = new Texture("floor-normal.jpg");
	Material* floor_m = new Material(mainShader);
	floor_m->addColorTex(floorAlbedoTex);
	floor_m->addNormalTex(floorNormalTex);
	floor_m->setTileU(20);
	floor_m->SetTileV(20);

	Model* plane = new Model();
	plane->loadMesh("plane.obj");
	plane->loadMaterial(floor_m);
	m_Models["floor"] = plane;


	Texture* tenguColorTex = new Texture("tengu-color.png");
	Texture* tenguNormalTex = new Texture("tengu-normal.png");
	Material* tengu_m = new Material(mainShader);
	tengu_m->addColorTex(tenguColorTex);
	tengu_m->addNormalTex(tenguNormalTex);


	Model* demon = new Model();
	demon->loadMesh("tengu.obj");
	demon->loadMaterial(tengu_m);
	//demon->setActive(false);
	m_Models["demon"] = demon;

	m_MainCam.setProj(45.0, m_SWidth, m_SHeight);

	m_LightManager->addLight(new PointLight(glm::vec3(5.0, 3.0, 4.0), glm::vec3(1.0, 1.0, 1.0), 1.5, 1));
	//m_LightManager->addLight(new PointLight(glm::vec3(-4.0, 1.0, 2.0), glm::vec3(1.0, 0.5, 0.5), 1, 1));

	createVignette();

	Texture* depthTexture = new Texture(0, GL_DEPTH_COMPONENT16, 1024*2, 1024*2, 0, GL_DEPTH_COMPONENT, GL_FLOAT, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	m_Framebuffers["depthFBO"] = new Framebuffer(depthTexture,
		GL_DEPTH_ATTACHMENT, GL_FALSE, GL_FALSE);
	//m_Vignette->setTexture(m_Framebuffers["depthFBO"]->getTextureAttachment());
	m_Framebuffers["vignetteFBO"] = new Framebuffer(m_Vignette->getTexture(), GL_COLOR_ATTACHMENT0, GL_TRUE, GL_TRUE);

	
}
void Renderer::DrawScene() {

	//Shadow mapping pass
	bindFramebuffer("depthFBO");

	glClear(GL_DEPTH_BUFFER_BIT);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);


	glViewport(0, 0, 1024*2, 1024*2);
	

	//Setup light point of view
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, .1f, 15.0f);
	glm::mat4 lightView = glm::lookAt(m_LightManager->getLight(0)->getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightViewProj = lightProj * lightView;

	glCullFace(GL_FRONT);
	m_Models["floor"]->getMesh()->drawShadows(m_Shaders["BasicDepthShader"], lightProj, lightView);
	m_Models["demon"]->getMesh()->drawShadows(m_Shaders["BasicDepthShader"], lightProj, lightView);
	m_Models["box"]->getMesh()->drawShadows(m_Shaders["BasicDepthShader"], lightProj, lightView);

	//For all lit shaders...
	//Upload lightsSpaceMatrixes
	m_Shaders["basicShader"]->bind();
	m_Shaders["basicShader"]->setMat4("u_lightViewProj", lightViewProj);

	m_Shaders["basicShader"]->setMat4("u_lightViewProj", lightViewProj);
	m_Framebuffers["depthFBO"]->getTextureAttachment()->bind(5);
	m_Shaders["basicShader"]->unbind();




	//Final pass write to vignette texture
	bindFramebuffer("vignetteFBO");
	glCullFace(GL_BACK);

	m_MainCam.setProj(45.0f, m_SWidth, m_SHeight);

	glViewport(0, 0, m_SWidth, m_SHeight);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


	glEnable(GL_DEPTH_TEST);

	renderLights(true);

	render("floor");
	render("demon");
	render("box");




	//Render to texture for possprocessing

	bindFramebuffer();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	/*glClear(GL_COLOR_BUFFER_BIT);*/  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	m_Vignette->draw();

}

void Renderer::Init() {
	// glfw: initialize and configure
	  // ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	m_Window = glfwCreateWindow(m_SWidth, m_SHeight, &m_Name, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

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

void Renderer::LateInit()
{
	m_Shaders["UnlitBasicShader"] = new Shader("UnlitBasicShader.shader");
	m_Shaders["BasicDepthShader"] = new Shader("BasicDepthShader.shader");

	m_LightManager->init(m_Shaders["UnlitBasicShader"]);
}

void Renderer::Tick()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
		DrawScene();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Renderer::Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	m_MainCam.camMovement(window, m_DeltaTime);

	//WIP LIGHT CONTROLS
	Light* l = m_LightManager->getLight(0);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x-.5f, l->getPosition().y, l->getPosition().z));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x+.5f, l->getPosition().y, l->getPosition().z));
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

		m_MainCam.camRotation(xoffset, yoffset);
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

void Renderer::render()
{
	renderLights(true);

	for (auto& m : m_Models) {
		m.second->draw(m_MainCam.getProj(), m_MainCam.getView());
	}

}

void Renderer::render(std::string name) {

	Model* m = m_Models[name];
	m->draw(m_MainCam.getProj(), m_MainCam.getView());

}
void Renderer::bindFramebuffer() {
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


void Renderer::renderLights(bool enableGizmos)
{
	for (auto& shader : m_Shaders) {
		m_LightManager->uploadLightDataToShader(shader.second, m_MainCam.getView());
	}
	if (enableGizmos)
		m_LightManager->drawLights(m_MainCam.getProj(), m_MainCam.getView());

}

void Renderer::bindFramebuffer(std::string name) {
	m_Framebuffers[name]->bind();

}


