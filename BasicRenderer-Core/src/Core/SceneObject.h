#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

/// <summary>
/// Basic virtual object class that can be instanciated in a scene
/// </summary>
class SceneObject
{
protected:
	std::string m_Name;
	glm::vec3 m_Location;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_Transform;
	bool enabled;
	
	void setTransform();
public:
	
	SceneObject(const std::string na) : m_Name(na), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)),m_Transform(glm::mat4(1.0f)) , enabled(true){}

	SceneObject(const std::string na, glm::vec3 p) : m_Name(na), m_Location(p), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)) , enabled(true) {}

	SceneObject(glm::vec3 p) : m_Name(""), m_Location(p), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true) {}

	SceneObject() : m_Name(""), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true) {}

	virtual void draw(glm::mat4 proj, glm::mat4 view) = 0;

	virtual void setPosition(const glm::vec3 p);

	virtual inline glm::vec3  getPosition() { return m_Location; };

	virtual void setRotation(const glm::vec3 p);

	virtual inline glm::vec3  getRotation() { return m_Rotation; };

	virtual void setScale(const glm::vec3  s);

	virtual inline glm::vec3 getScale() { return m_Scale; }

	virtual inline void setActive(const bool  s) { enabled = s; }

	virtual inline bool isActive() { return enabled; }
};

