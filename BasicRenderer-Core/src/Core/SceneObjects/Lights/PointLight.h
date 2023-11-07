#pragma once
#include "Light.h"


class PointLight : public Light
{
private:
	float m_AreaOfInfluence; //In units


public:
	PointLight(glm::vec3 p, glm::vec3  c) :Light(p, c, 0), m_AreaOfInfluence(10.0f) {}
	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Area of incluence</param>
	PointLight(glm::vec3 p, glm::vec3  c, float a) :Light(p,c, 0), m_AreaOfInfluence(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Area of incluence</param>
	PointLight(const std::string na, glm::vec3 p, glm::vec3  c, float a) :Light(na,p, c, 0), m_AreaOfInfluence(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Area of incluence</param>
	PointLight(glm::vec3 p, glm::vec3  c, float i, float a) :Light(p,c, i, 0), m_AreaOfInfluence(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Area of incluence</param>
	PointLight(const std::string na, glm::vec3 p, glm::vec3  c, float i, float a) :Light(na,p, c, i, 0), m_AreaOfInfluence(a) {}

	inline void setAreaOfInfluence(float a) { m_AreaOfInfluence = a; }

	inline float getAreaOfInfluence() { return m_AreaOfInfluence; }

	inline glm::mat4 getLightTransformMatrix() { return viewProj; }
};

