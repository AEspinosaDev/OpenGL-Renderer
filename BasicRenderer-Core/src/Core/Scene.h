#pragma once
#include <unordered_map>
#include "Model.h"
#include <Core/Lights/Light.h>
#include <OGL-Graphics/SkyboxMesh.h>
#include <Core/Camera.h>


class Scene {
private:
	std::string										m_Name;

	std::unordered_map<std::string, SceneObject*>	m_Objects;
	std::unordered_map<std::string, Model*>			m_Models;
	std::unordered_map<std::string, Light*>			m_Lights;
	std::unordered_map<std::string, Camera*>		m_Cameras;

	SkyboxMesh*										m_Skybox;
	Camera*											m_ActiveCamera;
	bool											m_Fog;
	float											m_AmbientStrength;
	glm::vec3										m_AmbientColor;

	unsigned int									modelNameIndexCount;
	unsigned int									pointlightNameIndexCount;
	unsigned int									spotlightNameIndexCount;
	unsigned int									directionallightNameIndexCount;
	unsigned int									cameraNameIndexCount;



public:
	Scene(std::string name) :
		m_Name(name),
		m_Skybox(nullptr),
		m_Fog(false),
		m_AmbientStrength(0.1),
		m_AmbientColor(glm::vec3(1.0f)),
		modelNameIndexCount(0),
		pointlightNameIndexCount(0),
		spotlightNameIndexCount(0),
		directionallightNameIndexCount(0),
		cameraNameIndexCount(0)
	{
		Camera* m = new Camera();
		m_ActiveCamera = m;
		m_Cameras["test"] = m;
	}
	inline std::string getName() { return m_Name; }

	inline std::unordered_map<std::string, SceneObject*> getSceneObjects() { return m_Objects; }
	inline std::unordered_map<std::string, Model*> getModels() { return m_Models; }
	inline std::unordered_map<std::string, Light*> getLights() { return m_Lights; }
	inline std::unordered_map<std::string, Camera*> getCameras() { return m_Cameras; }

	inline SceneObject* getSceneObjectByName(std::string name) { return m_Objects[name]; }
	inline Model* getModelByName(std::string name) { return m_Models[name]; }
	inline Light* getLightByName(std::string name) { return m_Lights[name]; }
	inline Camera* getActiveCamera() { return m_ActiveCamera; }

	inline SkyboxMesh* getSkybox() { return m_Skybox; }
	inline void setSkybox(SkyboxMesh* skybox) { m_Skybox = skybox; }

	inline void setAmbientStrength(float n) { m_AmbientStrength = n; }
	inline float getAmbientStrength() { return m_AmbientStrength; }
	inline void setAmbientColor(glm::vec3 ambientColor) { m_AmbientColor = ambientColor; }
	inline glm::vec3 getAmbientColor() { return m_AmbientColor; }

	void add(SceneObject* obj);


};
