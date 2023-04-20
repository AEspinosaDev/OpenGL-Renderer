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

    unsigned int  m_AntialiasingSamples;

    Vignette* m_Vignette;
    SkyboxMesh* m_Skybox;
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
    Renderer(char name, unsigned int width, unsigned int height, AntialiasingType antialiasing) :

        m_Name(name),
        m_Window(nullptr),
        m_SWidth(width), 
        m_SHeight(height), 
        m_Vignette(nullptr),
        m_Skybox(nullptr),
        m_ShadowResolution(ShadowMapQuality::MID),
        m_AntialiasingSamples(antialiasing),
        m_LightManager(new LightManager(ShadowMapQuality::MID)),
        m_DeltaTime(0.0), 
        m_LastFrame(0.0), 
        m_lastX(width * .5f), 
		m_lastY(height * .5f)
    {}

    
    inline void setAntialiasingType(AntialiasingType n) { m_AntialiasingSamples =n; }
    inline AntialiasingType getAntialiasingType() { return (AntialiasingType)m_AntialiasingSamples;  }
    inline void setShadoMapQuality(ShadowMapQuality q) { m_ShadowResolution = q; }
    inline ShadowMapQuality getShadoMapQuality() { return (ShadowMapQuality)m_ShadowResolution; }
    inline void setSkybox(SkyboxMesh* skybox) { m_Skybox = skybox; }
    
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
    /// Read source fbo data and write it in destiny fbo given some parameters
    /// </summary>
    void blitFramebuffer(std::string src_name, std::string dst_name, GLbitfield mask, GLenum filter);

    /// <summary>
    ///  Read source fbo data and write it in destiny fbo given some parameters
    /// </summary>
    void blitFramebuffer(std::string src_name,unsigned int src_x_o, unsigned int src_y_o, unsigned int src_x_f, unsigned int src_y_f,
        std::string dst_name, unsigned int dst_x_o, unsigned int dst_y_o, unsigned int dst_x_f, unsigned int dst_y_f, GLbitfield mask, GLenum filter);

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

    /// <summary>
    /// Render scene skybox if its using one
    /// </summary>
    void renderSkybox();

    /// <summary>
    /// Render all objects normals in scene for debugging purposes
    /// </summary>
    void drawObjectNormals();


};

