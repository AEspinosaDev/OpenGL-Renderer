#pragma once
#include "Light.h"


class PointLight : public Light
{
private:
	float m_AttRadius;

	float constant = 0;
	float linear = 0;
	float quadratic = 0;

public:
	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Attenuation radious</param>
	PointLight(glm::vec3 p, glm::vec3  c, float a) :Light(p,c, 0), m_AttRadius(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="t">Attenuation radious</param>
	PointLight(const std::string na, glm::vec3 p, glm::vec3  c, float a) :Light(na,p, c, 0), m_AttRadius(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Attenuation radious</param>
	PointLight(glm::vec3 p, glm::vec3  c, float i, float a) :Light(p,c, i, 0), m_AttRadius(a) {}

	/// <summary>
	/// Instanciates a point light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World position</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Attenuation radious</param>
	PointLight(const std::string na, glm::vec3 p, glm::vec3  c, float i, float a) :Light(na,p, c, i, 0), m_AttRadius(a) {}

	inline void setAttenuation(float a) { m_AttRadius = a; }

	inline float getAttenuation() { return m_AttRadius; }

	inline glm::mat4 getLightTransformMatrix() { return viewProj; }
};

