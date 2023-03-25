#pragma once
#include <string>
#include <glm.hpp>

/// <summary>
/// Basic virtual object class that can be instanciated in a scene
/// </summary>
class SceneObject
{
protected:
	std::string m_Name;
	glm::vec3 m_Location;
	glm::vec3 m_Rotation;
	float m_Scale;
	bool enabled;
	

public:
	
	SceneObject(const std::string na) : m_Name(na), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(1), enabled(true){}

	SceneObject(const std::string na, glm::vec3 p) : m_Name(na), m_Location(p), m_Rotation(glm::vec3()), m_Scale(1), enabled(true) {}

	SceneObject(glm::vec3 p) : m_Name(""), m_Location(p), m_Rotation(glm::vec3()), m_Scale(1), enabled(true) {}

	SceneObject() : m_Name(""), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(1), enabled(true) {}

	virtual void draw(glm::mat4 proj, glm::mat4 view) = 0;

	virtual inline void setPosition(const glm::vec3 p) { m_Location = p; }

	virtual inline glm::vec3  getPosition() { return m_Location; };

	virtual inline void setRotation(const glm::vec3 p) { m_Rotation = p; }

	virtual inline glm::vec3  getRotation() { return m_Rotation; };

	virtual inline void setScale(const float  s) { m_Scale = s; }

	virtual inline float getScale() { return m_Scale; }

	virtual inline void setActive(const bool  s) { enabled = s; }

	virtual inline bool isActive() { return enabled; }
};

