#include "InputManager.h"
#include "Core/UI/UISettings.h"

void InputManager::onWindowResize(GLFWwindow* window, int width, int height) {

	Renderer* r = Renderer::getInstance();
	r->m_UtilParameters.lastWidth = r->m_SWidth;
	r->m_UtilParameters.lastHeight = r->m_SWidth;
	r->m_SWidth = width;
	r->m_SHeight = height;

	//Resize framebuffers
	for (auto& fbo : r->m_Framebuffers) {
		fbo.second->resize(width, height);
	}
}

void InputManager::onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {


	//BASIC APPLICATION CONTROLS
	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) {
		UILayer::editMode ? UILayer::editMode = false : UILayer::editMode = true;
	}
	//EXIT APP______________________________
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	Renderer* r = Renderer::getInstance();

	r->m_ActiveController->handleKeyboard(r->m_CurrentScene->getActiveCamera(), window, r->m_UtilParameters.deltaTime);

	//FULLSCREEN MODE__________________________________
	if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
		if (!r->m_UtilParameters.isFullscreen) {
			r->m_UtilParameters.isFullscreen = true;
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			r->m_UtilParameters.isFullscreen = false;
			glfwSetWindowMonitor(window, NULL, 45, 45, r->m_UtilParameters.lastWidth, r->m_UtilParameters.lastHeight, GLFW_DONT_CARE);
		}
		//m_UtilParameters.isFullscreen ? glfwFullscre
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
	if (UIManager::needsToHandleInput()) {
		Renderer* r = Renderer::getInstance();
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

			if (r->m_UtilParameters.firstMouse)
			{
				r->m_UtilParameters.mouselastX = xpos;
				r->m_UtilParameters.mouselastY = ypos;
				r->m_UtilParameters.firstMouse = false;
			}

			float xoffset = xpos - r->m_UtilParameters.mouselastX;
			float yoffset = r->m_UtilParameters.mouselastY - ypos; // reversed since y-coordinates go from bottom to top

			r->m_UtilParameters.mouselastX = xpos;
			r->m_UtilParameters.mouselastY = ypos;

			r->m_ActiveController->handleMouse(r->m_CurrentScene->getActiveCamera(), xoffset, yoffset);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
			r->m_UtilParameters.firstMouse = true;
		}
	}
}