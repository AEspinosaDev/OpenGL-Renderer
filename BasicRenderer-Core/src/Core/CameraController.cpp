
#include "CameraController.h"

void CameraController::handleKeyboard(Camera* camera,GLFWwindow* window, const float deltaTime) {
	switch (TYPE)
	{
	case ORBITAL:
		break;
		keyPressedOrbital();
	case WASD:
		moveWASD(camera,window, deltaTime);
		break;
	}
}
void CameraController::handleMouse(Camera* camera,float xoffset, float yoffset, GLboolean constrainPitch) {
	switch (TYPE)
	{
	case ORBITAL:
		break;
		rotateOrbital();
	case WASD:
		rotateWASD(camera, xoffset, yoffset, constrainPitch);
		break;
	}
}
void  CameraController::moveWASD(Camera* camera, GLFWwindow* window, const float deltaTime) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() - camera->m_Transform.getForward() * camera->speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() + camera->speed * camera->m_Transform.getForward());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() + glm::normalize(glm::cross(camera->m_Transform.getForward(), camera->m_Transform.getUp())) * camera->speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() - glm::normalize(glm::cross(camera->m_Transform.getForward(), camera->m_Transform.getUp())) * camera->speed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() - glm::normalize(camera->m_Transform.getUp()) * camera->speed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera->setPosition(camera->getPosition() + glm::normalize(camera->m_Transform.getUp()) * camera->speed);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera->getPosition() = glm::vec3(0.0f, 1.0f, 8.0f);
		camera->m_Transform.setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
		camera->m_Transform.setUp(glm::vec3(0.0f, 1.0f, 0.0f));

	}

	camera->view = glm::lookAt(camera->getPosition(), camera->getPosition() - camera->m_Transform.getForward(), camera->m_Transform.getUp());
}
void  CameraController::rotateWASD(Camera* camera ,float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= camera->mouseSensitivity;
	yoffset *= camera->mouseSensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	if (constrainPitch)
	{
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	camera->updateCameraVectors(camera->pitch, camera->yaw);
}