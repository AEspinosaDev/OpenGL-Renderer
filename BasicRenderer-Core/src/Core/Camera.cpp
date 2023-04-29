#include "Camera.h"


void Camera::camMovement(GLFWwindow* window, const float deltaTime) {

	//Movimiento
	//float cameraSpeed = speed * deltaTime;
	float cameraSpeed = 0.1f;
	//float cameraSpeed = 0.4F;
	const float angle = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		setPosition(getPosition() - m_Transform.getForward() * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		setPosition(getPosition() + cameraSpeed * m_Transform.getForward());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		setPosition(getPosition() + glm::normalize(glm::cross(m_Transform.getForward(), m_Transform.getUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		setPosition(getPosition() - glm::normalize(glm::cross(m_Transform.getForward(), m_Transform.getUp())) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		setPosition(getPosition() - glm::normalize(m_Transform.getUp()) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		setPosition(getPosition() + glm::normalize(m_Transform.getUp()) * cameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {//Se resetea la camara a la posicion de origen
		getPosition() = glm::vec3(0.0f, 1.0f, 8.0f);
		m_Transform.setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
		m_Transform.setUp(glm::vec3(0.0f, 1.0f, 0.0f));

	}
	//Actualizamos el valor de la matriz vista de la camara
	view = glm::lookAt(getPosition(), getPosition() - m_Transform.getForward(), m_Transform.getUp());
}
void Camera::camRotation(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors(pitch, yaw);
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

