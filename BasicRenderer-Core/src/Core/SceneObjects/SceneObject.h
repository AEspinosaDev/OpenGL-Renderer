#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum ObjectType {
	MODEL,
	LIGHT,
	CAMERA,
};

struct Transform {
private:
	glm::mat4 worldMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 forward;

	bool isDirty;
public:

	Transform(
		glm::mat4 worldMatrix = glm::mat4(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f)
		
	) :
		position(position),
		scale(scale),
		rotation(rotation),
		up(up),
		forward(forward),
		right(right),
		worldMatrix(worldMatrix),
		isDirty(true) {}

	void setPosition(glm::vec3 p) {
		position = p;
		isDirty = true;
	}

	glm::vec3 getPosition() {
		return position;
	}

	void setScale(glm::vec3 s) {
		scale = s;
		isDirty = true;
	}
	glm::vec3 getScale() {
		return scale;
	}
	void setRotation(glm::vec3 r) {
		rotation = r;
		isDirty = true;
	}
	glm::vec3 getRotation() {
		return rotation;
	}

	glm::vec3 getUp() {
		return up;
	}
	void setUp(glm::vec3 u) {
		up = glm::normalize(u);
	}

	glm::vec3 getForward() {
		return forward;
	}
	void setForward(glm::vec3 u) {
		forward = glm::normalize(u);
	}
	glm::vec3 getRight() {
		return right;
	}
	void setRight(glm::vec3 u) {
		right = glm::normalize(u);
	}
	glm::mat4 getWorldMatrix() {
		//if(glm::vec4(position,1.0f) == worldMatrix[3])
		if (isDirty) {
			worldMatrix = glm::mat4(1.0f);
			worldMatrix = glm::translate(worldMatrix, position);
			worldMatrix = glm::rotate(worldMatrix, rotation.x, glm::vec3(1, 0, 0));
			worldMatrix = glm::rotate(worldMatrix, rotation.y, glm::vec3(0, 1, 0));
			worldMatrix = glm::rotate(worldMatrix, rotation.z, glm::vec3(0, 0, 1));
			worldMatrix = glm::scale(worldMatrix, scale);
			isDirty = false;
		}
		return worldMatrix;
	}
	bool getIsDirty() { return isDirty; }

	void setIsDirty(bool op) { isDirty = op; }

};

/// <summary>
/// Basic virtual object class that can be instanciated in a scene
/// </summary>
class SceneObject
{
protected:
	std::string m_Name;

	Transform m_Transform;

	SceneObject* child;
	SceneObject* parent;

	ObjectType m_Type;

	unsigned int m_Clones;
	bool enabled;
	bool selected;

	friend class UIManager;

public:

	SceneObject(const std::string na, ObjectType t) : m_Name(na), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0), selected(false) {}

	SceneObject(const std::string na, glm::vec3 p, ObjectType t) : m_Name(na), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0), selected(false) {
		m_Transform.setPosition(p);
	}

	SceneObject(glm::vec3 p, ObjectType t) : m_Name(""), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0), selected(false) {
		m_Transform.setPosition(p);
	}

	SceneObject(ObjectType t) : m_Name(""), enabled(true),
		child(nullptr), parent(nullptr), m_Type(t), m_Clones(0), selected(false) {}

	~SceneObject() {
		delete child;
		delete parent;
	}
	
	virtual bool isSelected() { return selected; }

	virtual void setPosition(const glm::vec3 p) { m_Transform.setPosition(p); }

	virtual inline glm::vec3  getPosition() { return m_Transform.getPosition(); };

	virtual void setRotation(const glm::vec3 p) { m_Transform.setRotation(p); }

	virtual inline glm::vec3  getRotation() { return m_Transform.getRotation(); };

	virtual void setScale(const glm::vec3  s) { m_Transform.setScale(s); }

	virtual inline glm::vec3 getScale() { return m_Transform.getScale(); }

	virtual inline Transform* getTransform() { return &m_Transform; }

	virtual inline void setActive(const bool  s) { enabled = s; }

	virtual inline bool isActive() { return enabled; }

	virtual inline std::string getName() { return m_Name; }

	virtual inline void setName(std::string s) { m_Name = s; }

	virtual inline ObjectType getObjectType() { return m_Type; }

	virtual void setTransform(Transform t) { m_Transform = t; }

};

