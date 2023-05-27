#pragma once
//#include <GLFW/glfw3.h>
#include "Renderer.h"

class InputManager {
public:
	static void onWindowResize(GLFWwindow* window, int width, int height);
	static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void onMouseMoved(GLFWwindow* window, double xpos, double ypos);


};