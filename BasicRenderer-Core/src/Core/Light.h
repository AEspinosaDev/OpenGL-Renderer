#pragma once
#include "SceneObject.h"
#include "Mesh.h"


/// <summary>
/// Base light class
/// </summary>
class Light : public SceneObject
{
protected:
	const unsigned int type;
	glm::vec3  m_Color;
	float m_Intensity;




public:
	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Type</param>
	Light(glm::vec3 p, glm::vec3  c, const unsigned int t) :SceneObject(p), m_Color(c), m_Intensity(1), type(t) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Type</param>
	Light(const std::string na, glm::vec3 p, glm::vec3  c, const unsigned int t) :SceneObject(na,p), m_Color(c), m_Intensity(1), type(t) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Type</param>
	Light(glm::vec3 p, glm::vec3  c, float i, const unsigned int t) :SceneObject(p), m_Color(c), m_Intensity(i), type(t) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">Position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Type</param>
	Light(const std::string na, glm::vec3 p, glm::vec3  c, float i, const unsigned int t) :SceneObject(na,p), m_Color(c), m_Intensity(i), type(t) {}


	/*virtual void draw() = 0;*/

	/*virtual inline void setPosition(const Vec3 p);

	virtual Vec3 getPosition();

	virtual void setScale(const float  s);

	virtual inline float getScale();*/

	virtual inline void setColor(const glm::vec3  c) { m_Color = c; }

	virtual inline glm::vec3  getColor() { return m_Color; }

	virtual inline void setIntensity(float i) { m_Intensity = i; }

	virtual inline float getIntensity() { return m_Intensity; }


	inline const unsigned int getType() { return type; }

	void draw(glm::mat4 proj, glm::mat4 view);

};

