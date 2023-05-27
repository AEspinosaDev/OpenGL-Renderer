
#include "UIManager.h"
#include "Materials/PhysicalMaterial.h"

SceneObject* UIManager::m_SelectedObject = nullptr;

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

		float mainMenuHeight = UIManager::renderMainMenuBar();

		UIManager::renderExplorerPanel(windowFlags, ImVec2(0, mainMenuHeight), ImVec2(r->m_SWidth * 0.15, r->m_SHeight * .4f));

		UIManager::renderGlobalSettingsPanel(windowFlags, ImVec2(0, r->m_SHeight * .4f + mainMenuHeight), ImVec2(r->m_SWidth * 0.15f, (r->m_SHeight - mainMenuHeight) * .6f));

		UIManager::renderViewportPanel(windowFlags, ImVec2(r->m_SWidth * 0.15f, mainMenuHeight), ImVec2(r->m_SWidth * 0.65, r->m_SHeight - mainMenuHeight));

		UIManager::renderPropertiesPanel(windowFlags, ImVec2(r->m_SWidth * 0.8f, mainMenuHeight), ImVec2(r->m_SWidth * 0.2, r->m_SHeight - mainMenuHeight));

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
float UIManager::renderMainMenuBar() {
	Renderer* r = Renderer::getInstance();
	//ImVec4 menuBarClr = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

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

	return mainMenuHeight;
}

void  UIManager::renderViewportPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size) {
	Renderer* r = Renderer::getInstance();


	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(size, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, size);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.05f, 0.05f, 0.1f, 1.0f));
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImVec2(0.0f, 0.0f));


	ImGui::Begin("Viewport", NULL, windowFlags | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
	{
		//BasicRendererMenu
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("Show wireframe", "CTRL+W", false)) {
				/*wip*/
			}
			if (ImGui::MenuItem("Show normals", "CTRL+N", r->m_UtilParams.renderNormals)) {
				r->m_UtilParams.renderNormals = !r->m_UtilParams.renderNormals;
			}
			if (ImGui::MenuItem("Show tangents", "CTRL+T", r->m_UtilParams.renderTangents)) {
				r->m_UtilParams.renderTangents = !r->m_UtilParams.renderTangents;
			}

			ImGui::EndMenu();
		}

		ImGui::PopStyleColor();
		ImGui::EndMenuBar();

		ImGui::BeginChild("SceneRender");

		if (ImGui::IsWindowHovered())
			r->m_UtilParams.canControl = true;

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
void UIManager::renderExplorerPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size) {
	Renderer* r = Renderer::getInstance();
	SceneObject* selectedObj = nullptr;

	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(size, 1);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, size);
	ImGui::Begin("Scene Explorer", NULL, windowFlags);


	static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody;
	/*static ImGuiTableFlags flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
		| ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_SizingFixedFit;*/
		//ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 0.0f, MyItemColumnID_ID);
		//ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
		//ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
		//ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_PreferSortDescending, 0.0f, MyItemColumnID_Quantity);
		//ImGui::TableSetupColumn("Description", (flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Description);
		//ImGui::TableSetupColumn("Hidden", ImGuiTableColumnFlags_DefaultHide | ImGuiTableColumnFlags_NoSort);
		//ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);

	if (ImGui::BeginTable("2ways", 2, flags))
	{
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Active", ImGuiTableColumnFlags_WidthFixed, 3 * 18.0f);
		ImGui::TableHeadersRow();

		// Simple storage to output a dummy file-system.
		struct MyTreeNode
		{
			SceneObject* obj;
			const char* Name;
			const char* Type;
			int             ChildIdx;
			int             ChildCount;
			bool selected;
			static void DisplayNode(MyTreeNode* node, std::vector<MyTreeNode> all_nodes)
			{

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				const bool is_folder = (node->ChildCount > 0);
				if (is_folder)
				{
					bool open = ImGui::TreeNodeEx(node->Name, ImGuiTreeNodeFlags_SpanFullWidth);
					ImGui::TableNextColumn();
					ImGui::TextDisabled("--");
					if (open)
					{
						for (int child_n = 0; child_n < node->ChildCount; child_n++)
							DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
						ImGui::TreePop();
					}
				}
				else
				{

					if (ImGui::TreeNodeEx(node->obj->getName().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
						/*	node->selected = ImGui::IsItemClicked();
							if (ImGui::IsItemClicked()) {
								UIManager::setSelectedObj(node->obj, true);
							}*/
					};
					/*if (ImGui::Selectable("paco", true))
					{
					}*/
					ImGui::TableNextColumn();

					if (ImGui::SmallButton(node->obj->isActive() ? "true" : "false")) {
						node->obj->setActive(!node->obj->isActive());
						//std::cout << "selected";
						//selectedObj = node->obj;
					}
				}
			}
		};

		auto objs = r->m_CurrentScene->getSceneObjects();
		std::vector<MyTreeNode> nodes;
		int counter = 0;
		for (auto& obj : objs) {
			auto tpe = obj.second->getObjectType();
			const char* type = "paco";
			ImGui::PushID(counter);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::TreeNodeEx(obj.second->getName().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {
				//node->selected = ImGui::IsItemClicked();
				if (ImGui::IsItemClicked()) {
					if (UIManager::m_SelectedObject) UIManager::m_SelectedObject->selected = false;
					UIManager::m_SelectedObject = obj.second;
					obj.second->selected = true;
				}
			};
			/*if (ImGui::Selectable("paco", true))
			{
			}*/
			ImGui::TableNextColumn();
			if (ImGui::Button(obj.second->isActive() ? "true" : "false")) {
				obj.second->setActive(!obj.second->isActive());
				//std::cout << "selected";
				//selectedObj = node->obj;
				//ImGui::PopButtonRepeat();
			}
			ImGui::PopID();


			//nodes.push_back({ obj.second, obj.first.c_str(), "wip", 0, 0 ,false });
			//MyTreeNode::DisplayNode(&nodes[counter], nodes);
			//if (nodes[counter].obj->selected)  selectedObj = nodes[counter].obj;
			///*if (nodes[counter].selected) if(nodes[counter].obj->selected = true;
			//if (nodes[counter].selected) selectedObj = nodes[counter].obj;*/

			counter++;
		}




		ImGui::EndTable();
	}


	ImGui::PopStyleVar(2);
	ImGui::End();


}
void  UIManager::renderPropertiesPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size) {
	Renderer* r = Renderer::getInstance();

	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(size, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, size);

	const float PI = 3.14159265359f;


	//If selected item
	ImGui::Begin("Properties", NULL, windowFlags);
	if (UIManager::m_SelectedObject) {
		std::string str = UIManager::m_SelectedObject->getName();
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		ImGui::TextColored(ImVec4(0.0, 0.0, 1.0, 1.0), str.c_str());

		ImGui::Separator();
		ImGui::Text("Transform");
		ImGui::Separator();

		float position[3] = { m_SelectedObject->getPosition().x,
			m_SelectedObject->getPosition().y,
			m_SelectedObject->getPosition().z
		};
		if (ImGui::InputFloat3("Position", position)) {
			m_SelectedObject->setPosition(glm::vec3(position[0], position[1], position[2]));

		};
		float rotation[3] = { (m_SelectedObject->getRotation().x * 180) / PI,
		(m_SelectedObject->getRotation().y * 180) / PI,
		(m_SelectedObject->getRotation().z * 180) / PI
		};
		if (ImGui::InputFloat3("Rotation", rotation)) {
			m_SelectedObject->setRotation(glm::vec3((rotation[0] * PI) / 180, (rotation[1] * PI) / 180, (rotation[2] * PI) / 180));
		};
		float scale[3] = { m_SelectedObject->getScale().x,
		m_SelectedObject->getScale().y,
		m_SelectedObject->getScale().z
		};
		if (ImGui::InputFloat3("Scale", scale)) {
			m_SelectedObject->setScale(glm::vec3(scale[0], scale[1], scale[2]));
		};

		ImGui::Separator();

		if (m_SelectedObject->getObjectType() == MODEL) {
			ImGui::Text("Model");
			Model* model = dynamic_cast<Model*>(UIManager::m_SelectedObject);

			MeshBufferData data = model->getMesh()->getMeshBuffertData(0);
			ImGui::BeginTable("Mesh Details", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody);

			ImGui::TableSetupColumn("Mesh", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);

			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("File route");
			ImGui::TableNextColumn();
			ImGui::Text(model->getMesh()->getFileRoute().c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Triangle count");
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(data.triangles).c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Vertex count");
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(data.numVertices).c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Instanced");
			ImGui::TableNextColumn();
			ImGui::Text(model->getMesh()->isInstanced() ? "True" : "False");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Receive shadows");
			ImGui::TableNextColumn();
			bool shadows = model->getMesh()->getCastShadows();
			if (ImGui::Checkbox("Cast Shadows", &shadows)) { model->getMesh()->setCastShadows(shadows); };

			ImGui::EndTable();

			ImGui::Separator();
			ImGui::Text("Material");
			ImGui::Separator();

			for (size_t i = 0; i < model->getMesh()->getNumberOfMeshes(); i++)
			{
				if (model->getMaterialReference(i)->getShaderNameID() == "PhysicallyBasedShader") {
					PhysicalMaterial* mat = dynamic_cast<PhysicalMaterial*>(model->getMaterialReference(i));
					glm::vec3 albedo = mat->getAlbedoColor();
					float metallic = mat->getMetalness();
					float roughness = mat->getRoughness();
					bool receiveShadows = mat->getReceiveShadows();
					if (ImGui::ColorEdit3("Color", (float*)&albedo)) { mat->setAlbedoColor(albedo); };
					if (ImGui::DragFloat("Metalness", &metallic, 0.005f, 0.0f, 1.0f)) mat->setMetalness(metallic);
					if (ImGui::DragFloat("Roughness", &roughness, 0.005f, 0.0f, 1.0f)) mat->setRoughness(roughness);
					if (ImGui::Checkbox("Receive Shadows", &receiveShadows)) { mat->setReceiveShadows(receiveShadows); };


				}

			}


		}
		if (m_SelectedObject->getObjectType() == LIGHT) {
			ImGui::Text("Light");
			ImGui::Separator();

			Light* light = dynamic_cast<Light*>(UIManager::m_SelectedObject);
			float intensity = light->getIntensity();
			glm::vec3 color = light->getColor();
			bool castShadows = light->getCastShadows();
			if (ImGui::DragFloat("Intensity", &intensity, 0.005f, 0.0f, 99.0f)) light->setIntensity(intensity);
			if (ImGui::ColorEdit3("Color", (float*)&color)) { light->setColor(color); };
			if (ImGui::Checkbox("Cast Shadows", &castShadows)) { light->setCastShadows(castShadows); };
			/*if(light.getAt)
			float att = light->getAttenuation();
			if (ImGui::DragFloat("Attenuation", &att, 0.005f, 0.0f, 99.0f)) light->setAttenuation(att);*/
		}
	}

	
	ImGui::PopStyleVar(2);
	ImGui::End();
}
void UIManager::renderGlobalSettingsPanel(ImGuiWindowFlags windowFlags, ImVec2 pos, ImVec2 size) {
	Renderer* r = Renderer::getInstance();

	ImGui::SetNextWindowPos(pos, 0);
	ImGui::SetNextWindowSize(size, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, size);


	ImGui::Begin("Global Settings", NULL, windowFlags);
	ImGui::Text("Post processing");
	ImGui::Separator();
	if (ImGui::Checkbox("Enable post process", &r->m_Settings.postProcess));
	if (ImGui::Checkbox("Bloom", &r->m_Settings.ppEffects.bloom));
	if (ImGui::Checkbox("Gamma correction", &r->m_Settings.ppEffects.gammaCorrection));

	ImGui::Separator();
	ImGui::Text("Shadows");
	ImGui::Separator();
	if (ImGui::DragFloat("Shadows far", &r->m_Settings.shadowFarPlane, 1.0f, 0.0f, 200.0f));
	const char* res[] = { "VERY LOW", "LOW", "MID", "HIGH", "VERY HIGHT" };
	
	static int res_current = 2;
	if (ImGui::Combo("Shadows resolution", &res_current, res, IM_ARRAYSIZE(res))) {
		switch (res_current)
		{
		case 0:
			r->m_Settings.antialiasingSamples = ShadowMapQuality::VERY_LOW;
			break;
		case 1:
			r->m_Settings.antialiasingSamples = ShadowMapQuality::LOW;
			break;
		case 2:
			r->m_Settings.antialiasingSamples = ShadowMapQuality::MID;
			break;
		case 3:
			r->m_Settings.antialiasingSamples = ShadowMapQuality::HIGH;
			break;
		case 4:
			r->m_Settings.antialiasingSamples = ShadowMapQuality::VERY_HIGHT;
			break;
		
		}

		/*if (res_current != 0) {
			r->m_Framebuffers["msaaFBO"]->setTextureAttachmentSamples((AntialiasingType)r->m_Settings.antialiasingSamples);
		}*/
	};
	ImGui::Separator();
	ImGui::Text("Antialiasing");

	ImGui::Separator();
	const char* items[] = { "NONE", "MSAAx2", "MSAAx4", "MSAAx8", "MSAAx16", "OTHER" };
	static int item_current = 4;
	if (ImGui::Combo("Mode", &item_current, items, IM_ARRAYSIZE(items))) {
		switch (item_current)
		{
		case 0:
			r->m_Settings.antialiasingSamples = AntialiasingType::NONE;
			break;
		case 1:
			r->m_Settings.antialiasingSamples = AntialiasingType::MSAAx2;
			break;
		case 2:
			r->m_Settings.antialiasingSamples = AntialiasingType::MSAAx4;
			break;
		case 3:
			r->m_Settings.antialiasingSamples = AntialiasingType::MSAAx8;
			break;
		case 4:
			r->m_Settings.antialiasingSamples = AntialiasingType::MSAAx16;
			break;
		}

		if (item_current != 0) {
			r->m_Framebuffers["msaaFBO"]->setTextureAttachmentSamples((AntialiasingType)r->m_Settings.antialiasingSamples);
		}
	};
	ImGui::Separator();
	ImGui::Text("Misc");
	ImGui::Separator();
	ImGui::ColorEdit3("Clear Color", (float*)&r->m_Settings.clearColor); // Edit 3 floats representing a color
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Separator();
	ImGui::Text("Application average"); 
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::PopStyleVar(2);
	ImGui::End();
}
