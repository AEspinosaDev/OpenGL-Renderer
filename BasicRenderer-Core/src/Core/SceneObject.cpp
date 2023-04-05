#include "SceneObject.h"


void SceneObject::setTransform()
{
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::translate(m_Transform, m_Location);
	m_Transform = glm::scale(m_Transform, m_Scale);
	m_Transform = glm::rotate(m_Transform, m_Rotation.x, glm::vec3(1, 0, 0));
	m_Transform = glm::rotate(m_Transform, m_Rotation.y, glm::vec3(0, 1, 0));
	m_Transform = glm::rotate(m_Transform, m_Rotation.z, glm::vec3(0, 0, 1));
}

void SceneObject::setPosition(const glm::vec3 p) { m_Location = p; setTransform(); }
void SceneObject::setRotation(const glm::vec3 p) { m_Rotation = p; setTransform();}
void SceneObject::setScale(const glm::vec3  s) { m_Scale = s; setTransform();}

