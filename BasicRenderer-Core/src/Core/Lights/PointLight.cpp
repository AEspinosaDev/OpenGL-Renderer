#include "PointLight.h"
#include <ext/matrix_transform.hpp>
#include <ext/matrix_clip_space.hpp>

glm::mat4 PointLight::getLightTransformMatrix(glm::vec3 target) {

	glm::mat4 lightProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);

	glm::mat4 lightView = glm::lookAt(m_Location, target, glm::vec3(0.0f, 1.0f, 0.0f));
	viewProj = lightProj * lightView;

	return viewProj;
}