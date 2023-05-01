#pragma once
#include "Core/SceneObject.h"
#include "Texture.h"


/// <summary>
/// Base pure abstract light class
/// </summary>
class Light : public SceneObject
{
protected:
	const unsigned int type;
	glm::vec3  m_Color;
	float m_Intensity;
	Texture* m_ShadowText;

	glm::mat4 viewProj;
	bool castShadows;



public:
	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Type</param>
	Light(glm::vec3 p, glm::vec3  c, const unsigned int t) :SceneObject(p, ObjectType::LIGHT), m_Color(c), m_Intensity(1), type(t), m_ShadowText(nullptr), viewProj(glm::mat4(0.0f)), castShadows(true) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Type</param>
	Light(const std::string na, glm::vec3 p, glm::vec3  c, const unsigned int t) :SceneObject(na, p, ObjectType::LIGHT), m_Color(c), m_Intensity(1), type(t), m_ShadowText(nullptr), viewProj(glm::mat4(0.0f)), castShadows(true) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Type</param>
	Light(glm::vec3 p, glm::vec3  c, float i, const unsigned int t) :SceneObject(p, ObjectType::LIGHT), m_Color(c), m_Intensity(i), type(t), m_ShadowText(nullptr), viewProj(glm::mat4(0.0f)), castShadows(true) {}

	/// <summary>
	/// Instanciates a light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">Position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Type</param>
	Light(const std::string na, glm::vec3 p, glm::vec3  c, float i, const unsigned int t) :SceneObject(na, p, ObjectType::LIGHT), m_Color(c), m_Intensity(i), type(t), m_ShadowText(nullptr), viewProj(glm::mat4(0.0f)), castShadows(true) {}

	virtual inline void setColor(const glm::vec3  c) { m_Color = c; }

	virtual inline glm::vec3  getColor() { return m_Color; }

	virtual inline void setIntensity(float i) { m_Intensity = i; }

	virtual inline float getIntensity() { return m_Intensity; }

	virtual inline void setCastShadows(bool c) { castShadows = c; }

	virtual inline bool getCastShadows() { return castShadows; }

	virtual inline Texture* getShadowText() { return m_ShadowText; }

	virtual void setShadowText(Texture* t); 

	inline const unsigned int getType() { return type; }

	virtual glm::mat4 getLightTransformMatrix() = 0;


};

