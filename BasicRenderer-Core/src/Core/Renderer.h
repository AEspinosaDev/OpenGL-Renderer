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

class Renderer
{
private: 
    char m_Name;

	GLFWwindow* m_Window;

    unsigned int m_SWidth = 800;
    unsigned int m_SHeight = 600;

    Camera m_MainCam;

    LightManager* m_LightManager;
    unsigned int m_ShadowResolution;

    Vignette* m_Vignette;
    std::unordered_map<std::string,Framebuffer*> m_Framebuffers;

    std::unordered_map<std::string,Model*> m_Models;
    std::unordered_map<std::string,Shader*> m_Shaders;

    //Utility variables
    float m_lastX;
    float m_lastY;
    bool m_FirstMouse = true;
    float m_DeltaTime;
    float m_LastFrame;



public:
    Renderer(char name, unsigned int width, unsigned int height) :

        m_Name(name),
        m_Window(nullptr),
        m_SWidth(width), 
        m_SHeight(height), 
        m_Vignette(nullptr),
        m_ShadowResolution(2048),
        m_LightManager(new LightManager(2048)),
        m_DeltaTime(0.0), 
        m_LastFrame(0.0), 
        m_lastX(width * .5f), 
		m_lastY(height * .5f)
    {}
    
    /// <summary>
    /// Run application
    /// </summary>
    /// <param name="window"></param>
    /// <param name="key"></param>
    /// <param name="scancode"></param>
    /// <param name="action"></param>
    /// <param name="mods"></param>
    void Run(); 

    /// <summary>
    /// Implement here all instance of objects necessary to render the scene in the desired way
    /// </summary>
    void SetupScene();

	/// <summary>
	/// Implement her how the scene is going to be render
	/// </summary>
	void DrawScene();




private:
    /// <summary>
    /// Init OpenGL context
    /// </summary>
    void Init();

    void LateInit();

    /// <summary>
    /// Update renderer state
    /// </summary>
    void Tick();
    
    void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);
    
	void FramebufferResize_Callback(GLFWwindow* window, int width, int height);


    //Utilities

    /// <summary>
    /// Render all models and lights
    /// </summary>
    void render();

    /// <summary>
    /// Render single model given its name
    /// </summary>
    void render(std::string name);

    /// <summary>
    /// Bind standard framebuffer
    /// </summary>
    void bindFramebuffer();

    /// <summary>
    /// Bind specific framebuffer given its name
    /// </summary>
    void bindFramebuffer(std::string name);

    /// <summary>
    /// Creates a vignette to render to texture
    /// </summary>
    void createVignette();

    /// <summary>
    /// Uploads all lights data to shaders and if enabled render their gizmo meshes
    /// </summary>
    void renderLights(bool enableGizmos);

    /// <summary>
    /// Compute shadows on all lit shaders
    /// </summary>
    void computeShadows();

};

