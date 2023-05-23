#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Vignette.h>
#include <OGL-Graphics/Framebuffer.h>
#include <Core/LightManager.h>
#include <OGL-Graphics/SkyboxMesh.h>
#include <Core/SceneObjects/Lights/PointLight.h>
#include <Core/SceneObjects/Lights/DirectionalLight.h>
#include <Core/Materials/BasicPhongMaterial.h>
#include <map>
#include "Shader.h"
#include "SceneObjects/Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "SceneObjects/Model.h"
#include "Scene.h"
#include "UIManager.h"
#include "InputManager.h"
#include "CameraController.h"


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
	bool vsync;
	float secondCounter;
	unsigned int fps;
	float mouselastX;
	float mouselastY;
	bool firstMouse;
	float deltaTime;
	float lastFrame;
	glm::vec4 clearColor;
	unsigned int lastWidth;
	unsigned int lastHeight;
};

class Renderer
{
private:
	//Singleton 
	static Renderer* m_InstancePtr;

	std::string m_Name;
	GLFWwindow* m_Window;
	unsigned int m_SWidth = 800;
	unsigned int m_SHeight = 600;

	//LightManager* m_LightManager;
	unsigned int m_ShadowResolution;
	unsigned int  m_AntialiasingSamples;

	const char* GLSL_VERSION = "#version 460";
	std::unordered_map<std::string, Shader*> m_Shaders;
	std::unordered_map<std::string, Framebuffer*> m_Framebuffers;
	std::unordered_map<std::string, Scene*> m_Scenes;
	Scene* m_CurrentScene;
	std::vector<CameraController> m_Controllers;
	CameraController* m_ActiveController;
	
	//PostProcess
	Vignette* m_Vignette;
	bool m_PossProcess;
	PossProcessEffects m_PPEffects;
	
	//Utility variables
	UtilityParameters m_UtilParameters;

	//Friends
	friend class InputManager;
	friend class UIManager;
	friend class LightManager;

	Renderer(std::string name, unsigned int width, unsigned int height, AntialiasingType antialiasing) :

		m_Name(name),
		m_Window(nullptr),
		m_SWidth(width),
		m_SHeight(height),
		m_Vignette(nullptr),
		m_PossProcess(false),
		m_ShadowResolution(ShadowMapQuality::MID),
		m_AntialiasingSamples(antialiasing),
		m_CurrentScene(nullptr),
		m_ActiveController(nullptr)
	{
		init();
		lateInit();
	}

public:
	Renderer(const Renderer& obj)
		= delete;

	/// <summary>
	/// Get renderer singleton. Instance defaults are 1200x900 and MSAAx16 aliasing. 
	/// </summary>
	static inline Renderer* getInstance() {
		if (!m_InstancePtr)
			m_InstancePtr = new Renderer("Renderer", 1200, 900, AntialiasingType::MSAAx16);
		return m_InstancePtr;
	}

#pragma region getters & setters
	inline void setAntialiasingType(AntialiasingType n) { m_AntialiasingSamples = n; }
	inline AntialiasingType getAntialiasingType() { return (AntialiasingType)m_AntialiasingSamples; }
	inline void setShadoMapQuality(ShadowMapQuality q) { m_ShadowResolution = q; }
	inline ShadowMapQuality getShadoMapQuality() { return (ShadowMapQuality)m_ShadowResolution; }
	inline glm::vec4 getClearColor() { return  m_UtilParameters.clearColor; }
	inline void setClearColor(glm::vec4 c) { m_UtilParameters.clearColor = c; }
	inline void setGammaCorrection(bool op) { op ? m_PPEffects.gammaCorrection = true : m_PPEffects.gammaCorrection = false; }
	inline void setVsync(bool op) { m_UtilParameters.vsync = op; }
	inline bool getVsync() { return m_UtilParameters.vsync; }
	void setWindowTitle(std::string name);
	inline std::string getWindowTitle() { return m_Name; }
	void setSize(unsigned int w, unsigned int h);
#pragma endregion
#pragma region main functions
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

	/// <summary>
	/// Terminates application process. Handles all memory erasings
	/// </summary>
	void terminate();
#pragma endregion
#pragma region core functions

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
	void renderShadows();

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
#pragma endregion
