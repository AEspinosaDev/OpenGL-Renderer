
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

		UIManager::renderExplorerPanel(windowFlags, ImVec2(0, mainMenuHeight), ImVec2(r->m_SWidth * 0.15, r->m_SHeight * .5f));

		UIManager::renderGlobalSettingsPanel(windowFlags, ImVec2(0, r->m_SHeight * .5f + mainMenuHeight), ImVec2(r->m_SWidth * 0.15f, (r->m_SHeight - mainMenuHeight) * .5f));

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
		if (ImGui::MenuItem("Focus on selection", "F")) {
			if (UIManager::m_SelectedObject) {
				r->m_CurrentScene->getActiveCamera()->focusOnTarget(UIManager::m_SelectedObject->getPosition());
			}
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
			if (ImGui::MenuItem("Enable gizmos", "CTRL+G", r->m_Settings.enableGizmos)) {
				r->m_Settings.enableGizmos = !r->m_Settings.enableGizmos;
			}
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
			(ImTextureID)r->m_Resources.framebuffers["viewportFBO"]->getTextureAttachment()->getID(),
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


	static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody/*| ImGuiTableFlags_BordersH*/;


	if (ImGui::BeginTable("2ways", 2, flags, ImVec2(size.x, size.y * 0.6f)))
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


	ImGui::SeparatorText("Ambient Light");
	glm::vec3 a_color = r->m_CurrentScene->getAmbientColor();
	float a_intensity = r->m_CurrentScene->getAmbientStrength();
	if (ImGui::ColorEdit3("Color", (float*)&a_color)) { r->m_CurrentScene->setAmbientColor(a_color); };
	if (ImGui::DragFloat("Intensity", &a_intensity, 0.005, 0.0f, 1.0f)) { r->m_CurrentScene->setAmbientStrength(a_intensity); };
	ImGui::SeparatorText("Fog");


	ImGui::SeparatorText("Skybox");
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


	//If selected item
	ImGui::Begin("Properties", NULL, windowFlags);
	/*ImGui::Image(r->m_Resources.framebuffers["highLightFBO"]->getTextureAttachment() ? (void*)r->m_Resources.framebuffers["highLightFBO"]->getTextureAttachment()->getID() : nullptr, ImVec2(size.x , size.x ));
	ImGui::Image(r->m_Resources.framebuffers["dilationFBO"]->getTextureAttachment() ? (void* )r->m_Resources.framebuffers["dilationFBO"]->getTextureAttachment()->getID() : nullptr, ImVec2(size.x , size.x ));*/
	if (UIManager::m_SelectedObject) {
		std::string str = UIManager::m_SelectedObject->getName();
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		ImGui::TextColored(ImVec4(0.0, 0.0, 1.0, 1.0), str.c_str());

		ImGui::SeparatorText("Transform");
		const float PI = 3.14159265359f;

		float position[3] = { m_SelectedObject->getPosition().x,
			m_SelectedObject->getPosition().y,
			m_SelectedObject->getPosition().z
		};
		if (ImGui::DragFloat3("Position", position, 0.1f)) {
			m_SelectedObject->setPosition(glm::vec3(position[0], position[1], position[2]));

		};
		float rotation[3] = { (m_SelectedObject->getRotation().x * 180) / PI,
		(m_SelectedObject->getRotation().y * 180) / PI,
		(m_SelectedObject->getRotation().z * 180) / PI
		};
		if (ImGui::DragFloat3("Rotation", rotation, 0.1f)) {
			m_SelectedObject->setRotation(glm::vec3((rotation[0] * PI) / 180, (rotation[1] * PI) / 180, (rotation[2] * PI) / 180));
		};
		float scale[3] = { m_SelectedObject->getScale().x,
		m_SelectedObject->getScale().y,
		m_SelectedObject->getScale().z
		};
		if (ImGui::DragFloat3("Scale", scale, 0.1f)) {
			m_SelectedObject->setScale(glm::vec3(scale[0], scale[1], scale[2]));
		};
		//ImGui::SameLine()


		if (m_SelectedObject->getObjectType() == MODEL) {
			ImGui::SeparatorText("Model");
			Model* model = dynamic_cast<Model*>(UIManager::m_SelectedObject);

			MeshBufferData data = model->getMesh()->getMeshBuffertData(0);
			ImGui::BeginTable("Mesh Details", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody);

			ImGui::TableSetupColumn("Mesh", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("");

			//ImGui::table

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("File route");
			ImGui::TableNextColumn();
			ImGui::Text(model->getMesh()->getFileRoute().c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Triangle count");
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(data.numFaces).c_str());

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
			if (ImGui::Checkbox("", &shadows)) { model->getMesh()->setCastShadows(shadows); };

			ImGui::EndTable();

			ImGui::SeparatorText("Material");

			ImGui::BeginTable("Mesh Details", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody);
			ImGui::TableSetupColumn("Material", ImGuiTableColumnFlags_NoHide);
			for (size_t i = 0; i < model->getMesh()->getNumberOfMeshes(); i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				std::string str = "ID " + std::to_string(i) + " - " + model->getMaterialReference(i)->getShaderNameID();
				ImGui::Text(str.c_str());
				ImGui::Separator();
				bool transparent = model->getMaterialReference(i)->getTransparency();
				if (ImGui::Checkbox("Transparent", &transparent)) { model->getMaterialReference(i)->setTransparency(transparent); }
				const char* faceVisibility[] = { "FRONT", "BACK", "BOTH" };
				static int currentFaceVisibility = model->getMaterialReference(i)->getParameters().faceVisibility;
				if (ImGui::Combo("Face visibility", &currentFaceVisibility, faceVisibility, IM_ARRAYSIZE(faceVisibility))) {
					switch (currentFaceVisibility)
					{
					case 0:
						model->getMaterialReference(i)->setFaceVisibility(FaceVisibility::FRONT);
						break;
					case 1:
						model->getMaterialReference(i)->setFaceVisibility(FaceVisibility::BACK);
						break;
					case 2:
						model->getMaterialReference(i)->setFaceVisibility(FaceVisibility::BOTH);
						break;

					}
				};
				const char* blending[] = { "NORMAL", "ADDITIVE", "CUSTOM" };
				static int currentBlending = model->getMaterialReference(i)->getParameters().blendingType;
				if (ImGui::Combo("Blending function", &currentBlending, blending, IM_ARRAYSIZE(blending))) {
					switch (currentBlending)
					{
					case 0:
						model->getMaterialReference(i)->setBlending(BlendingType::NORMAL);
						break;
					case 1:
						model->getMaterialReference(i)->setBlending(BlendingType::ADDITIVE);
						break;
					case 2:
						model->getMaterialReference(i)->setBlending(BlendingType::CUSTOM);
						break;

					}
				};


				if (model->getMaterialReference(i)->getShaderNameID() == "PhysicallyBasedShader") {
					//str += "Physical Material";
					//ImGui::Text("Physical Material");
					PhysicalMaterial* mat = dynamic_cast<PhysicalMaterial*>(model->getMaterialReference(i));
					glm::vec3 albedo = mat->getAlbedoColor();
					float metallic = mat->getMetalness();
					float roughness = mat->getRoughness();
					float ao = mat->getAO();
					float op = mat->getOpacity();
					bool receiveShadows = mat->getReceiveShadows();
					if (!mat->getAlbedoText()) {
						if (ImGui::ColorEdit3("Albedo", (float*)&albedo)) { mat->setAlbedoColor(albedo); };
					}
					else {
						ImGui::Image(mat->getAlbedoText() ? (void*)mat->getAlbedoText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Albedo");
					}
					if (!mat->getOpacityMask()) {
						if (ImGui::DragFloat("Opacity", &op, 0.005f, 0.0f, 1.0f)) { mat->setOpacity(op); };
					}
					else {
						ImGui::Image(mat->getOpacityMask() ? (void*)mat->getOpacityMask()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Opacity");
					}
					switch (mat->getMaskPreset())
					{
					case _NONE:
						if (!mat->getMetalnessText()) {
							if (ImGui::DragFloat("Metalness", &metallic, 0.005f, 0.0f, 1.0f)) mat->setMetalness(metallic);
						}
						else {
							ImGui::Image(mat->getMetalnessText() ? (void*)mat->getMetalnessText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Metalness");
						}
						if (!mat->getRoughnessText()) {
							if (ImGui::DragFloat("Roughness", &roughness, 0.005f, 0.0f, 1.0f)) mat->setRoughness(roughness);
						}
						else {
							ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Roughness");
						}
						if (!mat->getAOText()) {
							if (ImGui::DragFloat("Ambient Occ", &ao, 0.005f, 0.0f, 1.0f)) mat->setAO(ao);
						}
						else {
							ImGui::Image(mat->getAOText() ? (void*)mat->getAOText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Ambient Occ");
						}
						break;
					case UNITY_HDRP:
						ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Unity HDRP Mask");
						break;
					case UNITY_URP:
						break;
					case UNREAL_ENGINE_4:
						ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .15f, size.x * .15f)); ImGui::SameLine(); ImGui::Text("Unreal 4 Mask");
						break;
					default:
						break;
					}



					if (ImGui::Checkbox("Receive Shadows", &receiveShadows)) { mat->setReceiveShadows(receiveShadows); };
					if (ImGui::TreeNode("Texture info"))
					{
						ImGui::Text("Albedo");
						ImGui::Image(mat->getAlbedoText() ? (void*)mat->getAlbedoText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
						ImGui::Text("Normal");
						ImGui::Image(mat->getNormalText() ? (void*)mat->getNormalText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
						switch (mat->getMaskPreset())
						{
						case _NONE:
							ImGui::Text("Roughness");
							ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							ImGui::Text("Metalness");
							ImGui::Image(mat->getMetalnessText() ? (void*)mat->getMetalnessText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							ImGui::Text("Ambient Occ");
							ImGui::Image(mat->getAOText() ? (void*)mat->getAOText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							break;
						case UNITY_HDRP:
							ImGui::Text("Mask");
							ImGui::Text("R:Metallic G:Occlusion A:Roughness");
							ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							break;
						case UNITY_URP:
							ImGui::Text("Roughness");
							ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							break;
						case UNREAL_ENGINE_4:
							ImGui::Text("Mask");
							ImGui::Text("R:Occlusion G:Roughness B:Metallic");
							ImGui::Image(mat->getRoughnessText() ? (void*)mat->getRoughnessText()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));
							break;
						default:
							break;
						}
						ImGui::Text("Opacity");
						ImGui::Image(mat->getOpacityMask() ? (void*)mat->getOpacityMask()->getID() : nullptr, ImVec2(size.x * .5f, size.x * .5f));

						ImGui::TreePop();
					}
					ImGui::Separator();

				}
				if (model->getMaterialReference(i)->getShaderNameID() == "BasicPhongShader") {}
				if (model->getMaterialReference(i)->getShaderNameID() == "UnlitBasicShader") {}
			}
			ImGui::EndTable();

		}
		if (m_SelectedObject->getObjectType() == LIGHT) {
			ImGui::SeparatorText("Light");

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
		if (m_SelectedObject->getObjectType() == CAMERA) {
			ImGui::SeparatorText("Camera");

			Camera* cam = dynamic_cast<Camera*>(UIManager::m_SelectedObject);
			float far = cam->getFar();
			float near = cam->getNear();
			float fov = cam->getFOV();
			float speed = cam->getSpeed();

			bool isMainCamera = cam == r->m_CurrentScene->getActiveCamera();

			if (ImGui::Checkbox("Main camera", &isMainCamera)) { 
				r->m_CurrentScene->setActiveCamera(cam->getName());
			};
			if (ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 10.0f)) cam->setSpeed(speed);
			if (ImGui::DragFloat("Near", &near, 0.05f, 0.0f, 10.0)) cam->setNear(near);
			if (ImGui::DragFloat("Far", &far, 0.1f, 0.0f, 9999.0f)) cam->setFar(far);
			if (ImGui::DragFloat("Field of view", &fov, 0.1f, 0.0f, 160.0f)) cam->setFOV(fov);
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
	ImGui::SeparatorText("Post processing");
	if (ImGui::Checkbox("Enable post process", &r->m_Settings.postProcess));
	if (ImGui::Checkbox("Bloom", &r->m_Settings.ppEffects.bloom));
	if (ImGui::Checkbox("Gamma correction", &r->m_Settings.ppEffects.gammaCorrection));

	ImGui::SeparatorText("Shadows");
	if (ImGui::DragFloat("Far plane", &r->m_Settings.shadowFarPlane, 1.0f, 0.0f, 200.0f));
	const char* res[] = { "VERY LOW", "LOW", "MID", "HIGH", "VERY HIGHT" };

	static int res_current = 2;
	if (ImGui::Combo("Resolution", &res_current, res, IM_ARRAYSIZE(res))) {
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
			r->m_Resources.framebuffers["msaaFBO"]->setTextureAttachmentSamples((AntialiasingType)r->m_Settings.antialiasingSamples);
		}*/
	};
	ImGui::SeparatorText("Antialiasing");

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
			r->m_Resources.framebuffers["msaaFBO"]->setTextureAttachmentSamples((AntialiasingType)r->m_Settings.antialiasingSamples);
		}
	};


	ImGui::SeparatorText("Misc");
	ImGui::ColorEdit3("Clear Color", (float*)&r->m_Settings.clearColor); // Edit 3 floats representing a color
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Separator();
	ImGui::Text("Application average");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	ImGui::PopStyleVar(2);
	ImGui::End();
}
