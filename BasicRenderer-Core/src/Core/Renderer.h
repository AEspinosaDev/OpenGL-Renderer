#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "Core/Model.h"
#include <Vignette.h>
#include <OGL-Graphics/Framebuffer.h>
#include <Core/LightManager.h>
#include <OGL-Graphics/SkyboxMesh.h>
#include "Scene.h"

enum AntialiasingType {
	NONE = 0,
	MSAAx2 = 2,
	MSAAx4 = 4,
	MSAAx8 = 8,
	MSAAx16 = 16
};
enum ShadowMapQuality {
	VERY_LOW = 512,
	LOW = 1080,
	MID = 2048,
	HIGH = 4096,
	VERY_HIGHT = 8192
};

struct PossProcessEffects {
	bool gammaCorrection;
	bool bloom;
};

struct UtilityParameters {
	bool isFullscreen;

	float secondCounter;
	unsigned int fps;
	float mouselastX;
	float mouselastY;
	bool firstMouse;
	float deltaTime;
	float lastFrame;
	glm::vec4 clearColor;
};

class Renderer
{
private:
	std::string m_Name;

	GLFWwindow* m_Window;

	unsigned int m_SWidth = 800;
	unsigned int m_SHeight = 600;
	
	LightManager* m_LightManager;
	unsigned int m_ShadowResolution;
	unsigned int  m_AntialiasingSamples;

	std::unordered_map<std::string, Framebuffer*> m_Framebuffers;
	std::unordered_map<std::string, Scene*> m_Scenes;
	Scene* m_CurrentScene;
	std::unordered_map<std::string, Shader*> m_Shaders;
	//PostProcess
	Vignette* m_Vignette;
	bool m_PossProcess;
	PossProcessEffects m_PPEffects;
	//Utility variables
	UtilityParameters m_UtilParameters;


public:
	Renderer(std::string name, unsigned int width, unsigned int height, AntialiasingType antialiasing) :

		m_Name(name),
		m_Window(nullptr),
		m_SWidth(width),
		m_SHeight(height),
		m_Vignette(nullptr),
		m_PossProcess(false),
		m_ShadowResolution(ShadowMapQuality::MID),
		m_AntialiasingSamples(antialiasing),
		m_LightManager(new LightManager(ShadowMapQuality::MID)),

		m_CurrentScene(nullptr)
	{
		m_PPEffects.gammaCorrection = true;
		m_PPEffects.bloom = false;

		m_UtilParameters.isFullscreen = false;
		m_UtilParameters.secondCounter = 0;
		m_UtilParameters.fps = 0;
		m_UtilParameters.clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		m_UtilParameters.firstMouse = true;
		m_UtilParameters.deltaTime = 0.0;
		m_UtilParameters.lastFrame = 0.0;
		m_UtilParameters.mouselastX = width * .5f;
		m_UtilParameters.mouselastY = height * .5f;

		init();
		lateInit();
	}


	inline void setAntialiasingType(AntialiasingType n) { m_AntialiasingSamples = n; }
	inline AntialiasingType getAntialiasingType() { return (AntialiasingType)m_AntialiasingSamples; }
	inline void setShadoMapQuality(ShadowMapQuality q) { m_ShadowResolution = q; }
	inline ShadowMapQuality getShadoMapQuality() { return (ShadowMapQuality)m_ShadowResolution; }
	inline glm::vec4 getClearColor() { return  m_UtilParameters.clearColor; }
	inline void setClearColor(glm::vec4 c) { m_UtilParameters.clearColor = c; }
	inline void setGammaCorrection(bool op) { op ? m_PPEffects.gammaCorrection = true : m_PPEffects.gammaCorrection = false; }

	/// <summary>
	/// Run application
	/// </summary>
	void run();

	/// <summary>
	/// Implement her how the scene is going to be render
	/// </summary>
	void renderScene();

	/// <summary>
	/// Creates a new empty scene and add it to the scene map
	/// </summary>
	void addScene(Scene* sc);

	/// <summary>
	/// Gets scene reference by name
	/// </summary>
	Scene* getScene(std::string sceneName);

	void setCurrentScene(std::string sceneName);

	void setPostProcessPass(bool op);


private:
	/// <summary>
	/// Init OpenGL context
	/// </summary>
	void init();

	/// <summary>
	/// Init and compile basic and built in shaders an other openGL objs
	/// </summary>
	void lateInit();

	/// <summary>
	/// Upload to client and gpu necessary data to render
	/// </summary>
	void cacheData();

	/// <summary>
	/// Update renderer state
	/// </summary>
	void tick();

	void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);

	void FramebufferResize_Callback(GLFWwindow* window, int width, int height);


	//Utilities

	/// <summary>
	/// Render all models and lights
	/// </summary>
	void renderSceneObjects();

	/// <summary>
	/// Render model
	/// </summary>
	void renderModel(Model* m);


	/// <summary>
   /// Uploads all lights data to shaders and if enabled render their gizmo meshes
   /// </summary>
	void renderAndCacheLights(bool enableGizmos);

	/// <summary>
	/// Compute shadows on all lit shaders
	/// </summary>
	void computeShadows();

	/// <summary>
	/// Render scene skybox if its using one
	/// </summary>
	void renderSkybox();

	/// <summary>
	/// Render all objects normals in scene for debugging purposes
	/// </summary>
	void renderObjectNormals();

	/// <summary>
	/// 
	/// </summary>
	void possProcessPass();

	/// <summary>
	/// Bind standard framebuffer
	/// </summary>
	void bindFramebuffer();

	/// <summary>
	/// Bind specific framebuffer given its name
	/// </summary>
	void bindFramebuffer(std::string name);

	/// <summary>
	/// Read source fbo data and write it in destiny fbo given some parameters
	/// </summary>
	void blitFramebuffer(std::string src_name, std::string dst_name, GLbitfield mask, GLenum filter);

	/// <summary>
	///  Read source fbo data and write it in destiny fbo given some parameters
	/// </summary>
	void blitFramebuffer(std::string src_name, unsigned int src_x_o, unsigned int src_y_o, unsigned int src_x_f, unsigned int src_y_f,
		std::string dst_name, unsigned int dst_x_o, unsigned int dst_y_o, unsigned int dst_x_f, unsigned int dst_y_f, GLbitfield mask, GLenum filter);

	/// <summary>
	/// Read source fbo data and write it in standard framebuffer given some parameters
	/// </summary>
	void blitFramebuffer(std::string src_name, GLbitfield mask, GLenum filter);

	/// <summary>
	/// Creates a vignette to render to texture
	/// </summary>
	void createVignette();

	void profile();
};

