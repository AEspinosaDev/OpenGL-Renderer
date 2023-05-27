#include "InputManager.h"

void InputManager::onWindowResize(GLFWwindow* window, int width, int height) {

	Renderer* r = Renderer::getInstance();
	r->m_UtilParams.lastWidth = r->m_SWidth;
	r->m_UtilParams.lastHeight = r->m_SWidth;
	r->m_SWidth = width;
	r->m_SHeight = height;

	//Resize framebuffers
	for (auto& fbo : r->m_Framebuffers) {
		fbo.second->resize(width, height);
	}
}

void InputManager::onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {


	//BASIC APPLICATION CONTROLS
	Renderer* r = Renderer::getInstance();

	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
		r->m_Settings.editMode ? r->m_Settings.editMode = false : r->m_Settings.editMode = true;
	}
	//EXIT APP______________________________
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (r->m_UtilParams.isMouseLeftPressed && r->m_UtilParams.canControl)
		r->m_ActiveController->handleKeyboard(r->m_CurrentScene->getActiveCamera(), window, r->m_UtilParams.deltaTime);

	//FULLSCREEN MODE__________________________________
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
		if (!r->m_Settings.isFullscreen) {
			r->m_Settings.isFullscreen = true;
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			r->m_Settings.isFullscreen = false;
			glfwSetWindowMonitor(window, NULL, 45, 45, r->m_UtilParams.lastWidth, r->m_UtilParams.lastHeight, GLFW_DONT_CARE);
		}
		//m_UtilParameters.isFullscreen ? glfwFullscre
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		if (UIManager::m_SelectedObject)
			r->m_CurrentScene->getActiveCamera()->focusOnTarget(UIManager::m_SelectedObject->getPosition());
	}
	if (UIManager::needsToHandleInput()) {
		//WIP LIGHT CONTROLS 
		Light* l = r->m_CurrentScene->getLights().begin()->second;

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x - .5f, l->getPosition().y, l->getPosition().z));
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x + .5f, l->getPosition().y, l->getPosition().z));
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x, l->getPosition().y + .5f, l->getPosition().z));
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) l->setPosition(glm::vec3(l->getPosition().x, l->getPosition().y - .5f, l->getPosition().z));
	}

}
void InputManager::onMouseMoved(GLFWwindow* window, double xpos, double ypos) {
	Renderer* r = Renderer::getInstance();

	//Pressing
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		r->m_UtilParams.isMouseLeftPressed = true;

		if (r->m_UtilParams.firstMouse)
		{
			r->m_UtilParams.mouselastX = xpos;
			r->m_UtilParams.mouselastY = ypos;
			r->m_UtilParams.firstMouse = false;
		}

		float xoffset = xpos - r->m_UtilParams.mouselastX;
		float yoffset = r->m_UtilParams.mouselastY - ypos; // reversed since y-coordinates go from bottom to top

		r->m_UtilParams.mouselastX = xpos;
		r->m_UtilParams.mouselastY = ypos;

		if (r->m_UtilParams.canControl)
			r->m_ActiveController->handleMouse(r->m_CurrentScene->getActiveCamera(), xoffset, yoffset);

	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		r->m_UtilParams.isMouseMiddlePressed = true;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
		r->m_UtilParams.isMouseRightPressed = true;
	}


	//Unpressing
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
		r->m_UtilParams.firstMouse = true;
		r->m_UtilParams.canControl = false;
		r->m_UtilParams.isMouseLeftPressed = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		r->m_UtilParams.isMouseMiddlePressed = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE) {
		r->m_UtilParams.isMouseRightPressed = false;
	}
}