#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum ObjectType {
	MODEL,
	LIGHT,
	CAMERA,
};

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

	SceneObject* child;
	SceneObject* parent;

	ObjectType m_Type;

	unsigned int m_Clones;

	void setTransform();
public:

	SceneObject(const std::string na, ObjectType t) : m_Name(na), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0) {}

	SceneObject(const std::string na, glm::vec3 p, ObjectType t) : m_Name(na), m_Location(p), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0) {}

	SceneObject(glm::vec3 p, ObjectType t) : m_Name(""), m_Location(p), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0) {}

	SceneObject(ObjectType t) : m_Name(""), m_Location(glm::vec3()), m_Rotation(glm::vec3()), m_Scale(glm::vec3(1.0f)), m_Transform(glm::mat4(1.0f)), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0) {}

	~SceneObject() {
		delete child;
		delete parent;
	}

	virtual void draw(glm::mat4 proj, glm::mat4 view) = 0;

	virtual void setPosition(const glm::vec3 p);

	virtual inline glm::vec3  getPosition() { return m_Location; };

	virtual void setRotation(const glm::vec3 p);

	virtual inline glm::vec3  getRotation() { return m_Rotation; };

	virtual void setScale(const glm::vec3  s);

	virtual inline glm::vec3 getScale() { return m_Scale; }

	virtual inline glm::mat4 getTransform() { return m_Transform; }

	virtual inline void setActive(const bool  s) { enabled = s; }

	virtual inline bool isActive() { return enabled; }

	virtual inline std::string getName() { return m_Name; }

	virtual inline void setName(std::string s) { m_Name = s; }

	virtual inline ObjectType getObjectType() { return m_Type; }
};

