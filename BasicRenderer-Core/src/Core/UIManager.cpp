
#include "UIManager.h"

void UIManager::initUIContext(GLFWwindow* window, const char* version) {

	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(version);
	//ImGui_ImplGlfw_InstallCallbacks(window);

}
void UIManager::update() {

	/*if (!UILayer::editMode) {
		Renderer::getInstance()->bindFramebuffer();
		if (UILayer::editMode) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}*/
	ImGuiIO& io = ImGui::GetIO();
	Renderer* r = Renderer::getInstance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//bool show_demo_window = true;
	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);


	//ImGui::End();

	// Scene Explorer
	{
		static float f = 0.0f;
		static int counter = 0;


		ImGui::SetNextWindowPos(ImVec2(0, 0), 0);
		ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.15, r->m_SHeight), 0);

		ImGui::Begin("Scene Explorer");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Scene Objects");
		ImGui::BeginTable("Scene Objects", 3);

		for (int item = 0; item < 14; item++)
		{
			ImGui::TableNextColumn();
			ImGui::Text("Item %d", item);
		}
		ImGui::EndTable();
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(r->m_SWidth * 0.15, 0), 0);
		ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.65, r->m_SHeight), 0);
		ImGui::Begin("Viewport");
		{
			ImGui::BeginChild("GameRender");

			float width = ImGui::GetContentRegionAvail().x;
			float height = ImGui::GetContentRegionAvail().y;

			r->m_RWidth = width;
			r->m_RHeight = height;
			ImGui::Image(
				(ImTextureID)r->m_Framebuffers["viewportFBO"]->getTextureAttachment()->getID(),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
		}
		ImGui::EndChild();
		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(r->m_SWidth * 0.8, 0), 0);
		ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.2, r->m_SHeight), 0);
		ImGui::Begin("Properties");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("Transform");
		ImGui::BeginTable("Scene Objects", 3);

		for (int item = 0; item < 14; item++)
		{
			ImGui::TableNextColumn();
			ImGui::Text("Item %d", item);
		}
		ImGui::EndTable();
		/*ImGui::Checkbox("Demo Window", &show_demo_window);  */    // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&m_UtilParameters.clearColor); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
		//ImGui::End();
	}

	// 3. Show another simple window.
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me"))
	//		show_another_window = false;
	//	ImGui::End();
	//}

	// Rendering
	/*int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(m_UtilParameters.clearColor.r,
		m_UtilParameters.clearColor.g,
		m_UtilParameters.clearColor.b,
		m_UtilParameters.clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);*/

	//io.WantCaptureMouse = false;
	//ImGui_ImplGlfw_InstallCallbacks()
}

void UIManager::render() {
	Renderer::getInstance()->bindFramebuffer();
	if (UILayer::editMode) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void  UIManager::implementMouseCB(GLFWwindow* w, double xpos, double ypos) {
	ImGui_ImplGlfw_CursorPosCallback(w, xpos, ypos);
}
void  UIManager::implementKeyboardCB(GLFWwindow* w, int key, int scancode, int action, int mods) {
	ImGui_ImplGlfw_KeyCallback(w, key, scancode, action, mods);
}
bool  UIManager::needsToHandleInput() {
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard)
		return true;
	else
		return false;
}
void  UIManager::terminateUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}