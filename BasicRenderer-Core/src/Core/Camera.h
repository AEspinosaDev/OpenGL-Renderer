#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "SceneObject.h"

class Camera: public SceneObject {
private:
	
	glm::mat4 view;
	glm::mat4 proj;

	float m_Fov;
	float near;
	float far;
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
	Camera(glm::vec3 p = glm::vec3(0.0f, 1.0f, 8.0f), glm::vec3 f = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : SceneObject(p,ObjectType::CAMERA), mouseSensitivity(0.1), yaw(-90.0f), pitch(0.0f), speed(10.0f),m_Fov(45.0f),near(.1f),far(100.0f) {
		setView();
	}

	inline void setFOV(float fov) { m_Fov = fov; }
	inline float getFOV() { return m_Fov; }
	inline void setProj(int width, int height) { proj = glm::perspective(glm::radians(m_Fov), (float)width / (float)height, near, far); }
	inline glm::mat4 getProj() { return proj; }
	inline glm::mat4 getView() { return view; }
	inline float getFar() { return far; }
	inline void setFar(float f) { far = f; }
	inline float getNear() { return near; }
	inline void setNear(float n) { near = n; }

	void camMovement(GLFWwindow* window, const float deltaTime);

	void camRotation(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);


};

