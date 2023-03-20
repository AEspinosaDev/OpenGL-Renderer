#pragma once
#include <vector>
#include <Core/PointLight.h>
/// <summary>
/// Class that manages all lights instances and data.
/// </summary>
class LightManager
{
private:
	std::vector<Light*> m_Lights;
	Mesh* m_PLightMesh;
	Mesh* m_SLightMesh;
	Material* m_DebugMat;

	int pointLightsNumber;
	int directionalLightsNumber;
	int spotLightsNumber;

public:
	LightManager() : m_Lights(std::vector<Light*>()), pointLightsNumber(0), 
		directionalLightsNumber(0), 
		spotLightsNumber(0),
		m_PLightMesh(nullptr),
		m_SLightMesh(nullptr),
		m_DebugMat(nullptr) {}
	
	inline Light* getLight(int index) {
		return m_Lights[index];
	}

	void init(Shader* s);

	/// <summary>
	/// Adds a light to the scene. Point, spot and directional lights can be added.
	/// </summary>
	void addLight(Light* l);

	void removeLight();

	void drawLights(glm::mat4 proj, glm::mat4 view);

	/// <summary>
	/// Uploads all active lights data to the given shader
	/// </summary>
	void uploadLightDataToShader(Shader* s, glm::mat4 view);


};

