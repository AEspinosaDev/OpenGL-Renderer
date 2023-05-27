#pragma once
#include <Utils/imgui/imgui.h>
#include <Utils/imgui/imgui_impl_opengl3.h>
#include <Utils/imgui/imgui_impl_glfw.h>
#include <iostream>
#include "Renderer.h"

class UIManager {
	static SceneObject* m_SelectedObject;
public:
	static void initUIContext(GLFWwindow* window, const char* version);
	static void update(); 
	static void render(); 
	static void implementMouseCB(GLFWwindow* w, double xpos, double ypos);
	static void implementKeyboardCB(GLFWwindow* w, int key, int scancode, int action, int mods);
	static bool needsToHandleInput();
	static void terminateUI();
private:
	//ImGUI Panels
	static float renderMainMenuBar();
	static void renderViewportPanel(ImGuiWindowFlags windowFlags,ImVec2 pos, ImVec2 size);
	static void renderExplorerPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size);
	static void renderPropertiesPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size);
	static void renderGlobalSettingsPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size);
	
};