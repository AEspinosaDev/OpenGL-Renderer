#pragma once
#include <Utils/imgui/imgui.h>
#include <Utils/imgui/imgui_impl_opengl3.h>
#include <Utils/imgui/imgui_impl_glfw.h>

class UIManager {

public:
	static void initUIContext(GLFWwindow* window, const char* version);
	static void update(); 
	static void render(); //duh
	static void implementMouseCB(GLFWwindow* w, double xpos, double ypos);
	static void implementKeyboardCB(GLFWwindow* w, int key, int scancode, int action, int mods);
	static bool needsToHandleInput();
	static void terminateUI();
	
};