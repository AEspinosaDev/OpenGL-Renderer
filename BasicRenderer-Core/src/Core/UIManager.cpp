
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

	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		ImVec4 menuBarClr = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


		float mainMenuHeight;
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.2f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
		ImGui::BeginMainMenuBar();
		mainMenuHeight = ImGui::GetWindowHeight();
		if (ImGui::BeginMenu("Main"))
		{
			if (ImGui::MenuItem("Quit", "Esc")) {
				glfwSetWindowShouldClose(r->m_Window, GLFW_TRUE);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Fullscreen mode", "F11")) {
				r->m_Settings.isFullscreen = true;
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				glfwSetWindowMonitor(r->m_Window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
			}

			ImGui::EndMenu();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndMainMenuBar();


		renderExplorerPanel(windowFlags, ImVec2(0, mainMenuHeight));
		renderViewportPanel(windowFlags, ImVec2(r->m_SWidth * 0.15, mainMenuHeight));
		renderPropertiesPanel(windowFlags, ImVec2(r->m_SWidth * 0.8, mainMenuHeight));



	}


}

void UIManager::render() {
	Renderer::getInstance()->bindFramebuffer();
	if (Renderer::getInstance()->m_Settings.editMode) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); else return;
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

void  UIManager::renderViewportPanel(ImGuiWindowFlags windowFlags, ImVec2 pos) {
	Renderer* r = Renderer::getInstance();



	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.65, r->m_SHeight - pos.y), 0);
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, r->m_SHeight - pos.y));

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);

	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.05f, 0.05f, 0.1f, 1.0f));
	ImGui::Begin("Viewport", NULL, windowFlags | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Render Settings")) {
			//IMGUI_DEMO_MARKER("Menu/Examples");
			ImGui::MenuItem("Main menu bar", NULL, true);
			ImGui::MenuItem("Console", NULL, true);
			ImGui::MenuItem("Log", NULL, true);
			ImGui::MenuItem("Simple layout", NULL, true);
			ImGui::MenuItem("Property editor", NULL, true);
			ImGui::MenuItem("Long text display", NULL, true);
			ImGui::EndMenu();
		}
		ImGui::PopStyleColor();
		ImGui::EndMenuBar();


		ImGui::BeginChild("SceneRender");

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
	ImGui::PopStyleVar(3);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::End();
}
void  UIManager::renderExplorerPanel(ImGuiWindowFlags windowFlags, ImVec2 pos) {
	Renderer* r = Renderer::getInstance();

	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.15, r->m_SHeight - pos.y), 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, r->m_SHeight - pos.y));
	ImGui::Begin("Scene Explorer", NULL, windowFlags);


	static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

	if (ImGui::BeginTable("2ways", 2, flags))
	{
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 2 * 18.0f);
		ImGui::TableHeadersRow();

		// Simple storage to output a dummy file-system.
		struct MyTreeNode
		{
			const char* Name;
			const char* Type;
			int             ChildIdx;
			int             ChildCount;
			static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				const bool is_folder = (node->ChildCount > 0);
				if (is_folder)
				{
					bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
					ImGui::TableNextColumn();
					ImGui::TextDisabled("--");
					//ImGui::TableNextColumn();
					//ImGui::TextUnformatted(node->Type);
					if (open)
					{
						for (int child_n = 0; child_n < node->ChildCount; child_n++)
							DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
						ImGui::TreePop();
					}
				}
				else
				{
					ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
					ImGui::TableNextColumn();
					//ImGui::TableNextColumn();
					ImGui::TextUnformatted(node->Type);
				}
			}
		};
		auto objs = r->m_CurrentScene->getSceneObjects();
		const int s = objs.size() + 1;



		MyTreeNode nodes[50];

		nodes[0] = { "Root",                         "Scene",         1, 4 };
		int counter = 1;
		for (auto& obj : objs) {
			auto tpe = obj.second->getObjectType();
			const char* type = "paco";
			switch (tpe)
			{
			case MODEL:
				type = "Model";
				break;
			case LIGHT:
				type = "Light";
				break;
			case CAMERA:
				type = "Camera";
				break;
			default:
				break;
			}
			nodes[counter] = { obj.first.c_str(), type,   1, 0 };
			counter++;
		}


		MyTreeNode::DisplayNode(&nodes[0], nodes);


		ImGui::EndTable();
	}


	ImGui::PopStyleVar(3);
	ImGui::End();

}
void  UIManager::renderPropertiesPanel(ImGuiWindowFlags windowFlags, ImVec2 pos) {
	Renderer* r = Renderer::getInstance();

	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(ImVec2(r->m_SWidth * 0.2, r->m_SHeight - pos.y), 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, r->m_SHeight - pos.y));
	ImGui::Begin("Properties", NULL, windowFlags);                          // Create a window called "Hello, world!" and append into it.
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
	static float f = 0.0f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&m_UtilParameters.clearColor); // Edit 3 floats representing a color

	//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//	counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::PopStyleVar(3);
	ImGui::End();
	//ImGui::End();
}
//void  UIManager::renderToolbar(ImGuiWindowFlags windowFlags) {}
//void  UIManager::renderGlobalSettingsPanel(ImGuiWindowFlags windowFlags) {}