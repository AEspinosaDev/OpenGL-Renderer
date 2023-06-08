#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Vignette.h>
#include <OGL-Graphics/Framebuffer.h>
#include <OGL-Graphics/SkyboxMesh.h>
#include <map>
#include "Shader.h"
#include "LightManager.h"
#include "SceneObjects/Lights/PointLight.h"
#include "SceneObjects/Lights/DirectionalLight.h"
#include "Materials/BasicPhongMaterial.h"
#include "Materials/UnlitBasicMaterial.h"
#include "SceneObjects/Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "Quad.h"
#include "SceneObjects/Model.h"
#include "Scene.h"
#include "UIManager.h"
#include "InputManager.h"
#include "CameraController.h"


class Renderer
{
private:
	//Singleton 
	static Renderer*									m_InstancePtr;
	std::string											m_Name;
	GLFWwindow*											m_Window;
	//Window size
	unsigned int										m_SWidth;
	unsigned int										m_SHeight;
	//Render aspect
	unsigned int										m_RWidth; 
	unsigned int										m_RHeight;

	const char*											GLSL_VERSION = "#version 460";

	struct Resources {
		std::unordered_map<std::string, Shader*>			shaders;
		std::unordered_map<std::string, Framebuffer*>		framebuffers;
		std::unordered_map<std::string, Scene*>				scenes;
		std::vector<CameraController>						controllers;
		std::unordered_map<std::string, Texture*>			textures;
		std::unordered_map<std::string, Material*>			materials;
		std::unordered_map<std::string, Model*>				models;
		std::unordered_map<std::string, Mesh*>				primitives;


	};
	Resources											m_Resources;
	Scene*												m_CurrentScene;
	CameraController*									m_ActiveController;
	
	Vignette*											m_Vignette;

	
	struct PossProcessEffects {
		bool												gammaCorrection;
		bool												bloom;
	};

	struct UtilityParameters {
		//General
		float												secondCounter;
		unsigned int										fps;
		float												deltaTime;
		float												lastFrame;
		unsigned int										lastWidth;
		unsigned int										lastHeight;
		bool												renderNormals;
		bool												renderTangents;
		//Inputs
		float												mouselastX;
		float												mouselastY;
		bool												firstMouse;
		bool												canControl;
		bool												isMouseLeftPressed;
		bool												isMouseRightPressed;
		bool												isMouseMiddlePressed;
		bool												isMouseMiddleScrolled;


		UtilityParameters() {
			secondCounter = 0;
			fps = 0;
			firstMouse = true;
			deltaTime = 0.0;
			lastFrame = 0.0;
			mouselastX = 0;
			mouselastY = 0;
			lastWidth = 0;
			lastHeight = 0;
			canControl = false;
			isMouseLeftPressed = false;
			isMouseRightPressed = false;
			isMouseMiddlePressed = false;
			isMouseMiddleScrolled = false;
			renderNormals = false;
			renderTangents = false;
		}
	};


	struct UISettings {
		bool												paco; /*WIP*/
		UISettings() {
			paco = false;
		}
	};

	struct GlobalSettings {
		bool												isFullscreen;
		bool												vsync;
		unsigned int										shadowResolution;
		float												shadowFarPlane;
		unsigned int										antialiasingSamples;
		bool												postProcess;
		PossProcessEffects									ppEffects;
		bool												editMode;
		bool												enableGizmos;
		glm::vec4											clearColor;
		UISettings											UI_Settings;

		GlobalSettings() {
			isFullscreen = false;
			vsync = true;
			shadowResolution = ShadowMapQuality::MID;
			shadowFarPlane = 100.0;
			antialiasingSamples = AntialiasingType::MSAAx16;
			postProcess = false;
			ppEffects.bloom = false;
			ppEffects.gammaCorrection = true;
			editMode = true;
			clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
			UI_Settings;
			enableGizmos = true;
		}
	};

	//Settings
	GlobalSettings										m_Settings;
	UtilityParameters									m_UtilParams;

	//Friends
	friend class InputManager;
	friend class UIManager;
	friend class LightManager;

	Renderer(std::string name, unsigned int width, unsigned int height) :

		m_Name(name),
		m_Window(nullptr),
		m_SWidth(width),
		m_SHeight(height),
		m_RWidth(width),
		m_RHeight(height),
		m_Vignette(nullptr),
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
	/// Get renderer singleton. Instance defaults are 1200x900 and for settings go to Renderer_Core.h 
	/// </summary>
	static inline Renderer* getInstance() {
		if (!m_InstancePtr)
			m_InstancePtr = new Renderer("Renderer", 1200, 900);
		return m_InstancePtr;
	}

#pragma region getters & setters
	inline GlobalSettings* getGlobalSettings() { return &m_Settings; }
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

	void clear(bool color = true, bool depth = true, bool stencil = true);

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
	void cacheLights();
	void renderBillboards();
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

	void highlightPass(SceneObject* obj, const std::string defaultFBO);

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
