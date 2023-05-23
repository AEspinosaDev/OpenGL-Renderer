#pragma once
#include "SceneObjects/Camera.h"


enum MovementType {
	ORBITAL,
	WASD,
};

//Can be inherited to achieve completely custom control over the camera
class CameraController {
protected:
	const MovementType TYPE;
	void moveWASD(Camera* camera, GLFWwindow* window, const float deltaTime);
	void rotateWASD(Camera* camera, float xoffset, float yoffset, GLboolean constrainPitch);
	void keyPressedOrbital() {/*WIP*/ }
	void rotateOrbital() {/*WIP*/ }

public:
	CameraController(MovementType typeOfMovement) :TYPE(typeOfMovement) {}

	inline MovementType getType() { return TYPE; }

	virtual void handleKeyboard(Camera* camera, GLFWwindow* window, const float deltaTime);
	virtual void handleMouse(Camera* camera, float xoffset, float yoffset, GLboolean constrainPitch = true);
	virtual void handleMouseScroll() {/*WIP*/ }

};
