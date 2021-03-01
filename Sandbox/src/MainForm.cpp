#include "MainForm.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2.h>
#include "ASSInterface/Utils/PlatformUtils.h"

static bool show_configuration_menu_bar = false;
static bool show_enrollment_menu_bar = false;
static bool show_control_entry_menu_bar = false;
static bool show_video1_menu_bar = false;
static bool show_video2_menu_bar = false;

MainForm::MainForm() : Layer("WhoIsLayer")
{	
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].Init(i + 1);
	}	
	
	managerFile = ASSInterface::File::Create();
	managerFile->CreateFolder(managerFile->GetFolderConfiguration());
	configurationDB = ASSInterface::Configuration::CreateConfigDatabase();
	//configurationDB->ParseToFile();
	configurationDB->ParseToObject();
	std::any anyCnn = configurationDB->GetParam("connect");
	std::any anyDb = configurationDB->GetParam("name");
	if (anyCnn.has_value() && anyDb.has_value()) {
		std::string cnn = std::any_cast<std::string>(anyCnn);
		std::string db = std::any_cast<std::string>(anyDb);
		dbMongo = ASSInterface::Database::Create(cnn); 
		dbMongo->SetDatabase(db); 
	}	
	
	transformImage = ASSInterface::TransformImage::Create();
	base64 = ASSInterface::Base64::Create();
	
	SetImageToTexture("assets/textures/photo.png", personSpecificationLastAction.txtGallery);
	SetImageToTexture("assets/textures/photo.png", personSpecificationLastAction.txtCapture);
	std::string dateDetect = ASSInterface::DateTime::Now();	
	strcpy(personSpecificationLastAction.date, dateDetect.c_str());

	SetImageToTexture("assets/textures/obiwan.png", personSpecificationDetect.txtGallery);
	SetImageToTexture("assets/textures/obiwan.png", personSpecificationDetect.txtCapture);	
	strcpy(personSpecificationDetect.date, dateDetect.c_str());
	strcpy(personSpecificationDetect.type, "Employee");
	strcpy(personSpecificationDetect.state, "Not autorized");

	LoadUnidentified();
}

void MainForm::OnAttach()
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].OnAttach();
	}
	
}

void MainForm::OnDetach()
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].OnDetach();
	}
	
}

void MainForm::OnUpdate(ASSInterface::Timestep ts)
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].OnUpdate(ts);
	}
	
}

void MainForm::OnEvent(ASSInterface::Event& event)
{
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].OnEvent(event);
	}
	
}

void MainForm::ShowVideo(bool* p_open, int channel)
{
	if (!videos[channel].GetFlowRunning())
	{
		videos[channel].StartThread(videos[channel].GetNameThreadRun(), *p_open);
	}
	videos[channel].ShowVideo(p_open);
}

void MainForm::ShowMenuEnrollment(bool* p_open)
{
	std::string dateEnroll = ASSInterface::DateTime::Now();
	
	strcpy(personSpecificationEnroll.date, dateEnroll.c_str());

	const char* items[] = { "Video 1", "Video 2" };
	static const char* current_item = NULL;
	static int currentIndex = -1;

	if (currentIndex == 0 && !show_video1_menu_bar)
	{
		currentIndex = -1;
	}

	if (currentIndex == 1 && !show_video2_menu_bar)
	{
		currentIndex = -1;
	}

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment", p_open))
	{
		ImGui::End();
		return;
	}
	
	uint32_t textureID = txtImage->GetRendererID();	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();	
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 140.0f) * 0.5f, 30.0f));
	ImGui::Image((void*)textureID, ImVec2{ 150.0f, 187.0f });
	//ImGui::Checkbox("File", &show_video_menu_bar); ImGui::SameLine();
	//ImGui::Checkbox("Video", &show_video_menu_bar);
	if (ImGui::Button("File"))
		OpenFile();
	ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##combo", current_item)) 
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				ImGui::SetItemDefaultFocus();
				switch (n)
				{
				case 0:
					if (!show_video1_menu_bar)

					{
						currentIndex = 0;
						show_video1_menu_bar = true;
					}

					break;
				case 1:
					if (!show_video2_menu_bar)
					{
						currentIndex = 1;
						show_video2_menu_bar = true;
					}

					break;
				default:
					break;
				}
				/*if (is_selected) {
					
				}*/

			}


		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("ID:"); ImGui::SameLine();
	ImGui::InputText("##id", personSpecificationEnroll.id, IM_ARRAYSIZE(personSpecificationEnroll.id));
	ImGui::Text("Edit Name:"); ImGui::SameLine();
	ImGui::InputText("##name", personSpecificationEnroll.name, IM_ARRAYSIZE(personSpecificationEnroll.name));
	ImGui::Text("Edit Document:"); ImGui::SameLine();
	ImGui::InputText("##document", personSpecificationEnroll.document, IM_ARRAYSIZE(personSpecificationEnroll.document));
	ImGui::Text("Edit Date:"); ImGui::SameLine();
	ImGui::InputText("##date", personSpecificationEnroll.date, IM_ARRAYSIZE(personSpecificationEnroll.date));
	ImGui::Spacing();
	if (ImGui::Button("Save")) {			
		SaveDataPerson(currentIndex);
	}
			
	ImGui::End();
}

void MainForm::ShowScreenHeadControlEntry() {
	std::string dateDetect = ASSInterface::DateTime::Now();
	const char* items[] = { "Video 1", "Video 2" };
	static const char* current_item = NULL;
	static int currentIndex = -1;

	if (currentIndex == 0 && !show_video1_menu_bar)
	{
		currentIndex = -1;
	}

	if (currentIndex == 1 && !show_video2_menu_bar)
	{
		currentIndex = -1;
	}

	ImGui::BeginGroup();
	ImGui::Text("Choose channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##combo", current_item)) 
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); 
			if (ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				ImGui::SetItemDefaultFocus();
				switch (n)
				{
				case 0:
					if (!show_video1_menu_bar)

					{
						currentIndex = 0;
						show_video1_menu_bar = true;
					}

					break;
				case 1:
					if (!show_video2_menu_bar)
					{
						currentIndex = 1;
						show_video2_menu_bar = true;
					}

					break;
				default:
					break;
				}
			}

		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Text("Operator:"); ImGui::SameLine();
	ImGui::Text("Bienvenido Martinez"); ImGui::SameLine();
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::Text(&dateDetect[0]);
	ImGui::EndGroup();
	ImGui::Separator();
	ImGui::Spacing();
}

void MainForm::ShowMenuControlEntry(bool* p_open)
{
		
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Control Entry", p_open))
	{		
		ImGui::End();
		return;
	}

	ShowScreenHeadControlEntry();

	ShowScreenLastAction();

	ShowScreenUnidentified();
	
	ShowScreenDetect();

	ImGui::End();
}

void MainForm::ShowScreenLastAction() {
	ImGui::BeginGroup();
	ImGui::Text("Last action: Authorize Entry");
	ImGui::Spacing();
	ImGui::EndGroup();

	ImGui::Image((void*)personSpecificationLastAction.txtGallery->GetRendererID(), ImVec2{ 100.0f, 125.0f });
	ImGui::SameLine();
	ImGui::Image((void*)personSpecificationLastAction.txtCapture->GetRendererID(), ImVec2{ 100.0f, 125.0f });

	ImGui::SameLine();
	ImGui::BeginGroup();	
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name", personSpecificationLastAction.name, IM_ARRAYSIZE(personSpecificationLastAction.name));
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##document", personSpecificationLastAction.document, IM_ARRAYSIZE(personSpecificationLastAction.document));
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##date", personSpecificationLastAction.date, IM_ARRAYSIZE(personSpecificationLastAction.date));
	ImGui::Text("Type Person:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::InputText("##type", personSpecificationLastAction.type, IM_ARRAYSIZE(personSpecificationLastAction.type),
		ImGuiInputTextFlags_AllowTabInput | (readOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Place:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##place", personSpecificationLastAction.place, IM_ARRAYSIZE(personSpecificationLastAction.place),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Match:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##match", personSpecificationLastAction.match, IM_ARRAYSIZE(personSpecificationLastAction.match),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##channel", personSpecificationLastAction.channel, IM_ARRAYSIZE(personSpecificationLastAction.channel),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("State:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##state", personSpecificationLastAction.state, IM_ARRAYSIZE(personSpecificationLastAction.state),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::EndGroup();
	ImGui::Separator();
	ImGui::Spacing();
}

void MainForm::LoadUnidentified() {
	for (int i = 0; i < 7; i++)
	{
		std::string dateDetect = ASSInterface::DateTime::Now();
		std::string path = "assets/textures/" + std::to_string(i) + ".png";
		PersonSpecification pSpec;
		strcpy(pSpec.date, dateDetect.c_str());		
		SetImageToTexture(path, pSpec.txtCapture);
		listUnidentified.push_back(pSpec);
	}
}

void MainForm::ShowScreenDetect() {
	const char* items[] = { "Office 1", "Office 2" };
	static const char* current_item = NULL;

	ImGui::BeginChild("Detect", ImVec2(0, 120), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8);
	ImGui::PushFont(font_current);

	ImGui::Image((void*)personSpecificationDetect.txtGallery->GetRendererID(), ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::Image((void*)personSpecificationDetect.txtCapture->GetRendererID(), ImVec2{ 60.0f, 75.0f });

	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name", personSpecificationDetect.name, IM_ARRAYSIZE(personSpecificationDetect.name));
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##document", personSpecificationDetect.document, IM_ARRAYSIZE(personSpecificationDetect.document));
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##date", personSpecificationDetect.date, IM_ARRAYSIZE(personSpecificationDetect.date));
	ImGui::Text("Type Person:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::InputText("##type", personSpecificationDetect.type, IM_ARRAYSIZE(personSpecificationDetect.type),
		ImGuiInputTextFlags_AllowTabInput | (readOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Place:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##place", personSpecificationDetect.place, IM_ARRAYSIZE(personSpecificationDetect.place),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Match:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##match", personSpecificationDetect.match, IM_ARRAYSIZE(personSpecificationDetect.match),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##channel", personSpecificationDetect.channel, IM_ARRAYSIZE(personSpecificationDetect.channel),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("State:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##state", personSpecificationDetect.state, IM_ARRAYSIZE(personSpecificationDetect.state),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Place:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combo", current_item)) {
		for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Button("Authorize entry");
	ImGui::Button("Authorize departure");
	ImGui::EndGroup();
	ImGui::PopFont();

	ImGui::EndChild();
}

void MainForm::ShowScreenUnidentified() {

	std::string dateDetect = ASSInterface::DateTime::Now();
	static int hue = 140;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	ImGui::BeginChild("Not found", ImVec2(0, 180), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImFont* font_current = ImGui::GetFont();
	
	for (int line = 0; line < 7; line++)
	{
		
		uint32_t textureID = listUnidentified[line].txtCapture->GetRendererID(); 		
		ImGui::BeginGroup();
		ImGui::Text("Not found");
		ImGui::Spacing();
		ImGui::Image((void*)textureID, ImVec2{ 60.0f, 75.0f });	
		ImGui::SetWindowFontScale(0.8);
		ImGui::PushFont(font_current);
		ImGui::Text(listUnidentified[line].date); //&dateDetect[0]
		ImGui::PopFont();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(col_area.x, col_area.y, col_area.z, 0.50f)); //(ImVec4)ImColor::HSV(0.2f, 0.5f, 0.3f));
		ImGui::Button("ENROLL");
		ImGui::PopStyleColor(1);
		ImGui::EndGroup();
		if (line + 1 != 7)
		{
			ImGui::SameLine();
		}
			
		
	}
		
	ImGui::EndChild();
}

void MainForm::OpenFile() {
	if (!show_video1_menu_bar)
	{
		std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile("Images (*.png)\0*.png\0");
		if (!filePaths.empty())
		{
			for (int i = 0; i < filePaths.size(); i++)
			{
				if (i == 0)
				{
					SetImageToTexture(filePaths[i], txtImage);
				}
				ASS_INFO("Files: {0}", filePaths[i]);
			}

		}
	}
	
}

void MainForm::SaveDataPerson(int channel)
{
	
	ASSInterface::EntitySpecification entSpec;
	entSpec.id = &personSpecificationEnroll.id[0];
	entSpec.name = &personSpecificationEnroll.name[0];
	entSpec.identification = &personSpecificationEnroll.document[0];
	entSpec.date = ASSInterface::DateTime::NowToLong();

	if (!entSpec.id.empty() && channel != -1)
	{
		std::vector<unsigned char> bufferImage = transformImage->Resize(videos[channel].GetDataRaw(),
			videos[channel].GetHeightImage(), videos[channel].GetWidthImage());

		unsigned int in_len = (unsigned int)bufferImage.size();

		entSpec.data = base64->base64_encode(&bufferImage[0], in_len);
		dbMongo->Add(entSpec);

		int width, height, channels;

		unsigned char* dataTemp = SOIL_load_image_from_memory(&bufferImage[0],
			(int)bufferImage.size(), &width, &height, &channels, SOIL_LOAD_RGB);

		SetImageToTexture(dataTemp, width, height, channels, txtImage);
	}
	

}

void MainForm::GetDataPerson(std::string id) {

	if (!id.empty())
	{
		dbMongo->Get(id.c_str());

		long dte = dbMongo->GetEntitySpecification().date;
		std::string strDate = ASSInterface::DateTime::LongToString(dte);
		strcpy(personSpecificationLastAction.id, dbMongo->GetEntitySpecification().id.c_str());
		strcpy(personSpecificationLastAction.name, dbMongo->GetEntitySpecification().name.c_str());
		strcpy(personSpecificationLastAction.document, dbMongo->GetEntitySpecification().identification.c_str());
		strcpy(personSpecificationLastAction.date, strDate.c_str());

		std::string imgData = dbMongo->GetEntitySpecification().data;
		std::string decImg = base64->base64_decode(imgData);
		std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);

		int width, height, channels;

		unsigned char* dataTemp = SOIL_load_image_from_memory(templateData,
			(int)cstr.size(), &width, &height, &channels, SOIL_LOAD_RGB);

		//SetImageToTexture(dataTemp, width, height, channels, txtImage1);

		readOnly = true;
	}
		
}

void MainForm::SetImageToTexture(const std::string& path, ASSInterface::Ref<ASSInterface::Texture2D>& texture)
{
	texture = ASSInterface::Texture2D::Create(path);
	
}

void MainForm::SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel, 
	ASSInterface::Ref<ASSInterface::Texture2D>& texture)
{
	texture = ASSInterface::Texture2D::Create(data, width, height, channel);
	
}


void MainForm::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();

	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("ASSDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	style.WindowMinSize.x = minWinSizeX;

	ShowMenuMain();

	ImGui::End();
}

void MainForm::ShowMenuMain()
{
	
	if (show_configuration_menu_bar) ShowMenuConfiguration(&show_configuration_menu_bar);

	if (show_video1_menu_bar) ShowVideo(&show_video1_menu_bar, 0);
	if (!show_video1_menu_bar) videos[0].StopThread(videos[0].GetNameThreadRun());
	if (show_video2_menu_bar) ShowVideo(&show_video2_menu_bar, 1);
	if (!show_video2_menu_bar) videos[1].StopThread(videos[1].GetNameThreadRun());


	if (show_enrollment_menu_bar) ShowMenuEnrollment(&show_enrollment_menu_bar);
	if (show_control_entry_menu_bar) ShowMenuControlEntry(&show_control_entry_menu_bar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Configuration", NULL, &show_configuration_menu_bar);
			if (ImGui::BeginMenu("Shoot Channel"))
			{
				ImGui::MenuItem("Video 1", NULL, &show_video1_menu_bar);
				ImGui::MenuItem("Video 2", NULL, &show_video2_menu_bar);				
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Exit")) ASSInterface::Application::Get().Close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Recognition"))
		{
			ImGui::MenuItem("Enrollment", NULL, &show_enrollment_menu_bar);
			ImGui::MenuItem("Control Entry", NULL, &show_control_entry_menu_bar);
			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}
}

void MainForm::ShowMenuConfiguration(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::Begin("Configuration", p_open);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Channel Parameters"))
		{
			if (ImGui::TreeNode("Select channel"))
			{
			
				const char* items[] = { "Video 1", "Video 2" };
				static const char* current_item = NULL;
				ImGui::PushItemWidth(100);				
				if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
						if (ImGui::Selectable(items[n], is_selected)) {
							current_item = items[n];
							if (is_selected) {
								ImGui::SetItemDefaultFocus();
								switch (n)
								{
								case 0:
									show_video1_menu_bar = true;
									break;
								case 1:
									show_video2_menu_bar = true;
									break;
								default:
									break;
								}
							}
								
						}
							
								
					}
					ImGui::EndCombo();
				}


				static int e = 0;
				ImGui::RadioButton("IP", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("File", &e, 1); ImGui::SameLine();
				ImGui::RadioButton("Device", &e, 2);
				ImGui::TreePop();
				ImGui::Separator();
			}

			if (ImGui::TreeNode("Face Processing"))
			{				
				static int i1 = 0;
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("Template quality", &i1, 1, 255);
				ImGui::TreePop();
				ImGui::Separator();
			}
			ImGui::EndTabItem();

		}
		if (ImGui::BeginTabItem("Database"))
		{
			ImGui::Text("This is the Database tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Global Parameters"))
		{
			ImGui::Text("This is the Global Parameters tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}
