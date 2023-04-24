#pragma once
#include <vector>
#include <Core/Lights/Light.h>
#include <OGL-Graphics/Mesh.h>
#include <Core/Materials/UnlitBasicMaterial.h>
/// <summary>
/// Class that manages all lights instances and data.
/// </summary>
class LightManager
{
private:
	std::vector<Light*> m_Lights;
	Mesh* m_PLightMesh;
	Mesh* m_DLightMesh;
	Mesh* m_SLightMesh;
	UnlitBasicMaterial* m_DebugMat;

	float m_AmbientStrength;
	glm::vec3 m_AmbientColor;

	unsigned int m_ShadowResolution;
	float m_ShadowFarPlane;

	const int MAX_LIGHTS = 32;
	int pointLightsNumber;
	int directionalLightsNumber;
	int spotLightsNumber;
	int lightsNumber;

public:
	LightManager(unsigned int shadowRes) : m_Lights(std::vector<Light*>()), pointLightsNumber(0), m_ShadowResolution(shadowRes), m_AmbientStrength(0.0),m_AmbientColor(glm::vec3(1.0,1.0,1.0)),
		directionalLightsNumber(0), 
		spotLightsNumber(0),
		lightsNumber(0),
		m_PLightMesh(nullptr),
		m_DLightMesh(nullptr),
		m_SLightMesh(nullptr),
		m_DebugMat(nullptr),
		m_ShadowFarPlane(100.0)
	{}
	
	inline Light* getLight(int index) {
		return m_Lights[index];
	}
	inline void setAmbientStrength(float n) { m_AmbientStrength = n; }
	inline float getAmbientStrength() { return m_AmbientStrength; }
	inline void setAmbientColor(glm::vec3 ambientColor ) { m_AmbientColor = ambientColor; }
	inline glm::vec3 getAmbientColor() { return m_AmbientColor; }
	inline int getLightsCount() { return lightsNumber; }
	inline float getShadowsFarPlane() { return m_ShadowFarPlane; }
	inline void setShadowsFarPlane(float f) { m_ShadowFarPlane=f; }
	inline Material* getDebugMaterial() { return m_DebugMat; }

	//void init(std::unordered_map<std::string, Shader*> shaders);
	void init();

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

