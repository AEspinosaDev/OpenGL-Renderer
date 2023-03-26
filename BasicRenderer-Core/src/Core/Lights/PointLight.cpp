#include "PointLight.h"
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

glm::mat4 PointLight::getLightTransformMatrix() {

	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, .1f, 15.0f);
	glm::mat4 lightView = glm::lookAt(m_Location, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	viewProj = lightProj * lightView;

	return viewProj;
}