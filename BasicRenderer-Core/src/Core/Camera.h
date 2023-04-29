#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "SceneObject.h"

class Camera: public SceneObject {
private:
	/*glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;*/
	glm::mat4 view;
	glm::mat4 proj;

	float m_Fov;
	float yaw;
	float pitch;
	float mouseSensitivity;
	float zoom;
	float speed;

	inline void setView() {
		view = glm::lookAt(getPosition(), getPosition() - m_Transform.getForward(), m_Transform.getUp());
	}
	inline void updateCameraVectors(float pitch, float yaw) {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_Transform.setForward(-glm::normalize(direction));
		setView();
	}
public:
	Camera(glm::vec3 p = glm::vec3(0.0f, 1.0f, 8.0f), glm::vec3 f = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : SceneObject(p,ObjectType::CAMERA), mouseSensitivity(0.1), yaw(-90.0f), pitch(0.0f), speed(10.0f),m_Fov(45.0f) {
		setView();
	}

	inline void setFOV(float fov) { m_Fov = fov; }

	inline float getFOV() { return m_Fov; }

	/*inline void setFront(glm::vec3 f) { m_CameraFront = f; }

	inline void setUp(glm::vec3 up) { m_CameraUp = up; }*/

	inline void setProj(int width, int height) { proj = glm::perspective(glm::radians(m_Fov), (float)width / (float)height, 0.1f, 100.0f); }

	/*inline glm::vec3 getFront() { return m_CameraFront; }

	inline glm::vec3 getUp() { return m_CameraUp; }*/

	inline glm::mat4 getView() { return view; }

	inline glm::mat4 getProj() { return proj; }

	void camMovement(GLFWwindow* window, const float deltaTime);

	void camRotation(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);

	void draw(glm::mat4 proj, glm::mat4 view){ //Draw gizmos 
	}

};

