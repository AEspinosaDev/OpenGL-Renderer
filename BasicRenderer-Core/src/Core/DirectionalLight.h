#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 m_Direction;

public:
	/// <summary>
	/// Instanciates a directional light
	/// </summary>
	/// <param name="p">World direction</param>
	/// <param name="c">Color</param>
	/// <param name="t">Attenuation radious</param>
	DirectionalLight(glm::vec3 d, glm::vec3  c, float a) :Light(glm::vec3(0.0, 5.0 ,0.0), c, 0), m_Direction(d) {}

	/// <summary>
	/// Instanciates a directional light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World direction</param>
	/// <param name="c">Color</param>
	/// <param name="t">Attenuation radious</param>
	DirectionalLight(const std::string na, glm::vec3 d, glm::vec3  c, float a) :Light(na, glm::vec3(0.0, 5.0, 0.0), c, 0), m_Direction(d) {}

	/// <summary>
	/// Instanciates a directional light
	/// </summary>
	/// <param name="p">World direction</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Attenuation radious</param>
	DirectionalLight(glm::vec3 d, glm::vec3  c, float i, float a) :Light(glm::vec3(0.0, 5.0, 0.0), c, i, 0), m_Direction(d) {}

	/// <summary>
	/// Instanciates a directional light
	/// </summary>
	/// <param name="na">Name or ID</param>
	/// <param name="p">World direction</param>
	/// <param name="c">Color</param>
	/// <param name="i">Intensity</param>
	/// <param name="t">Attenuation radious</param>
	DirectionalLight(const std::string na, glm::vec3 d, glm::vec3  c, float i, float a) :Light(na, glm::vec3(0.0, 5.0, 0.0), c, i, 0), m_Direction(d) {}

	inline void setDirection(glm::vec3 d) { m_Direction = d; }

	inline glm::vec3 getDirection() { return m_Direction; }

	glm::mat4 getLightTransformMatrix();
};

