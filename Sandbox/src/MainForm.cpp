#include "MainForm.h"
#include <imgui/imgui.h>
#include "imgui/imgui_internal.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2.h>
#include "ASSInterface/Utils/PlatformUtils.h"

static bool show_configuration_menu_bar = false;
static bool show_enrollment_menu_bar = false;
static bool show_control_entry_menu_bar = false;
static bool show_video1_menu_bar = false;
static bool show_video2_menu_bar = false;
static bool show_face_processing = false;
static bool show_face_tracking = false;
static bool show_face_enrollment = false;
static bool show_control_entry_match = false;
static bool show_params_channel = false;
static bool show_params_global_biometric = false;
static bool show_params_enroll_onthefly = false;
static bool show_params_database = false;
static bool show_unidentified = false;
static bool show_output_control = false;
static bool show_simple_overlay = false;


MainForm::MainForm() : Layer("WhoIsLayer")
{			
	lg = ASSInterface::Label::GetCurrentLanguage();
	managerFile = ASSInterface::File::Create();
	managerFile->CreateFolder(managerFile->GetFolderConfiguration());
	//transformImage = ASSInterface::TransformImage::Create();

	frmConfiguration = new FrameConfiguration(lg);
	dbMongo = ASSInterface::Database::Create(frmConfiguration->GetConnectDatabase());
	dbMongo->SetDatabase(frmConfiguration->GetNameDatabase());	
	frmEnrollment->SetDatabase(dbMongo);

	for (int i = 0; i < MAX_CHANNELS; i++)
	{				
		videos[i].Init(i + 1);
	}			
	
	frmControlEntry->SetViewChannel(&show_video1_menu_bar);
	frmControlEntry->SetViewChannel(&show_video2_menu_bar);
	frmEnrollment->SetViewChannel(&show_video1_menu_bar);
	frmEnrollment->SetViewChannel(&show_video2_menu_bar);


	//temp
	frmControlEntry->SetNameOperator("Angel Martinez");
	//base64 = ASSInterface::Base64::Create();
		
	SetImageToTexture("assets/textures/photo.png", frmControlEntry->GetPersonLasAction()->txtGallery);
	SetImageToTexture("assets/textures/photo.png", frmControlEntry->GetPersonLasAction()->txtCapture);
	std::string dateDetect = ASSInterface::DateTime::Now();
	strcpy(frmControlEntry->GetPersonLasAction()->date, dateDetect.c_str());

	SetImageToTexture("assets/textures/obiwan.png", personSpecificationDetect.txtGallery);
	SetImageToTexture("assets/textures/obiwan.png", personSpecificationDetect.txtCapture);	
	strcpy(personSpecificationDetect.date, dateDetect.c_str());
	strcpy(personSpecificationDetect.type, "Employee");
	strcpy(personSpecificationDetect.state, "Not autorized");

	frmControlEntry->SetPersonDetected(personSpecificationDetect);
	
	
	//SetImageToTexture("assets/textures/avatar.png", frmEnrollment->GetPersonEnrolled()->txtCapture);
			
	LoadUnidentified();

	progressBar = prgBar;
	
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
	if (indexUnidentified != -1)
	{
		frmEnrollment->SetEnabledCapture(false);
		frmEnrollment->GetPersonEnrolled()->txtCapture = frmUnidentified->GetPersonUnidentified(indexUnidentified)->txtCapture;
		frmEnrollment->GetPersonEnrolled()->bufferCapture.clear();
		frmEnrollment->GetPersonEnrolled()->bufferCapture.assign(&frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture[0], &frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture[0] + frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture.size());

		std::string id = std::to_string(indexUnidentified);
		strcpy(frmEnrollment->GetPersonEnrolled()->id, id.c_str());		
		indexUnidentified = -1;
	}
	strcpy(personSpecificationDetect.id, "-1");	 
	frmEnrollment->Show(p_open, personSpecificationDetect);
	std::string id = &personSpecificationDetect.id[0];
	if (id != "-1")
	{
		std::string state = &personSpecificationDetect.state[0];
		if (state == "Autorized")
		{
			frmOutputControl->SetPersonInside(personSpecificationDetect);
		}
		else
		{
			frmControlEntry->SetPersonDetected(personSpecificationDetect);
			
		}
		
		frmUnidentified->RemovePersonUnidentified(atoi(id.c_str()));

		//Temp
		int img = distribution(generator);
		std::string path = "assets/textures/" + std::to_string(img) + ".png";
		SetImageToTexture(path, txtWindowTip);
		timeStartWindowTip = clock();
		show_simple_overlay = true;
		ShowWindowTip(&show_simple_overlay);
		//

		*p_open = false;
	}
		
}

void MainForm::ShowMenuControlEntry(bool* p_open)
{
	strcpy(personSpecificationDetect.id, "-1");
	frmControlEntry->Show(p_open, personSpecificationDetect);

	std::string id = &personSpecificationDetect.id[0];
	if (id != "-1")
	{
		frmOutputControl->SetPersonInside(personSpecificationDetect);		
	}
}

void MainForm::LoadUnidentified() {
	for (int i = 0; i < 7; i++)
	{
		std::string dateDetect = ASSInterface::DateTime::Now();
		std::string path = "assets/textures/" + std::to_string(i) + ".png";
		PersonSpecification pSpec;
		strcpy(pSpec.date, dateDetect.c_str());				
		SetImageToTexture(path, pSpec.txtCapture);	

		unsigned char* data = nullptr;
		{			
			int width, height, channels;
			data = SOIL_load_image(path.c_str(), &width, &height, &channels, 0);
			int size = width * height * channels;
			pSpec.bufferCapture.assign(data, data + size);
		}

		frmControlEntry->SetPersonUnidentified(pSpec);
		frmUnidentified->SetPersonUnidentified(pSpec);
	}
}

//bool MainForm::SaveDataPerson(int channel)
//{
//	
//	ASSInterface::EntitySpecification entSpec;
//	entSpec.id = &frmEnrollment->GetPersonEnrolled()->id[0]; // personSpecificationEnroll.id[0]
//	entSpec.name = &frmEnrollment->GetPersonEnrolled()->name[0];
//	entSpec.identification = &frmEnrollment->GetPersonEnrolled()->document[0];
//	entSpec.date = ASSInterface::DateTime::NowToLong();
//	entSpec.type = &frmEnrollment->GetPersonEnrolled()->type[0];
//
//	if (!entSpec.id.empty() && channel != -1 && !entSpec.name.empty() && 
//		!entSpec.identification.empty() && !entSpec.type.empty())
//	{
//		std::vector<unsigned char> bufferImage = transformImage->Resize(videos[channel].GetDataRaw(),
//			videos[channel].GetHeightImage(), videos[channel].GetWidthImage());
//
//		unsigned int in_len = (unsigned int)bufferImage.size();
//
//		entSpec.data = base64->base64_encode(&bufferImage[0], in_len);
//		//dbMongo->Add(entSpec);
//
//		int width, height, channels;
//
//		unsigned char* dataTemp = SOIL_load_image_from_memory(&bufferImage[0],
//			(int)bufferImage.size(), &width, &height, &channels, SOIL_LOAD_RGB);
//		frmEnrollment->SetImageToTexture(dataTemp, width, height, channels);
//		return true;
//		
//	}
//	return false;
//}

//void MainForm::GetDataPerson(std::string id) {
//
//	if (!id.empty())
//	{
//		/*dbMongo->Get(id.c_str());
//
//		long dte = dbMongo->GetEntitySpecification().date;
//		std::string strDate = ASSInterface::DateTime::LongToString(dte);
//		strcpy(personSpecificationLastAction.id, dbMongo->GetEntitySpecification().id.c_str());
//		strcpy(personSpecificationLastAction.name, dbMongo->GetEntitySpecification().name.c_str());
//		strcpy(personSpecificationLastAction.document, dbMongo->GetEntitySpecification().identification.c_str());
//		strcpy(personSpecificationLastAction.date, strDate.c_str());
//
//		std::string imgData = dbMongo->GetEntitySpecification().data;
//		std::string decImg = base64->base64_decode(imgData);
//		std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
//		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);
//
//		int width, height, channels;
//
//		unsigned char* dataTemp = SOIL_load_image_from_memory(templateData,
//			(int)cstr.size(), &width, &height, &channels, SOIL_LOAD_RGB);*/
//
//	
//	}
//		
//}

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
	ShowStatusBar();
	ImGui::End();
}

void MainForm::ShowMenuMain()
{
	
	if (show_configuration_menu_bar) ShowMenuConfiguration(&show_configuration_menu_bar);

	if (show_video1_menu_bar) ShowVideo(&show_video1_menu_bar, 0);
	if (!show_video1_menu_bar) videos[0].StopThread(videos[0].GetNameThreadRun());
	if (show_video2_menu_bar) ShowVideo(&show_video2_menu_bar, 1);
	if (!show_video2_menu_bar) videos[1].StopThread(videos[1].GetNameThreadRun());
	if (show_face_processing) frmConfiguration->ShowParametersFaceProcessing(&show_face_processing);
	if (show_face_tracking) frmConfiguration->ShowParametersFaceTracking(&show_face_tracking);
	if (show_face_enrollment) frmConfiguration->ShowParametersEnrollmentProcessing(&show_face_enrollment);
	if (show_control_entry_match) frmConfiguration->ShowParametersControlEntryMatch(&show_control_entry_match);
	if (show_params_channel) frmConfiguration->ShowParametersChannel(&show_params_channel);
	if (show_params_global_biometric) frmConfiguration->ShowParametersBiometric(&show_params_global_biometric);
	if (show_params_enroll_onthefly) frmConfiguration->ShowParametersOnthefly(&show_params_enroll_onthefly);
	if (show_params_database) frmConfiguration->ShowParametersDatabase(&show_params_database);

	if (show_output_control) frmOutputControl->Show(&show_output_control);
	if (show_unidentified) frmUnidentified->Show(&show_unidentified, &show_enrollment_menu_bar, indexUnidentified);
	if (show_enrollment_menu_bar) ShowMenuEnrollment(&show_enrollment_menu_bar);
	if (show_control_entry_menu_bar) ShowMenuControlEntry(&show_control_entry_menu_bar);
	if (!show_enrollment_menu_bar) {
		frmEnrollment->SetEnabledCapture(true);		
		indexUnidentified = -1;
		frmEnrollment->CleanSpecification();
		SetImageToTexture("assets/textures/avatar.png", frmEnrollment->GetPersonEnrolled()->txtCapture);		
	}
		
	if (show_simple_overlay) ShowWindowTip(&show_simple_overlay);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Configuration")) {
				ImGui::MenuItem("All parameters", NULL, &show_configuration_menu_bar);
				if (ImGui::BeginMenu(ASSInterface::Label::GetLabel(LBL_GLOBAL_PARAMETERS, lg).c_str())) {
					ImGui::MenuItem("Global biometric parameters", NULL, &show_params_global_biometric);
					ImGui::MenuItem("Global enrollment on-the-fly parameters", NULL, &show_params_enroll_onthefly);
					ImGui::MenuItem("Database parameters", NULL, &show_params_database);					
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Channel parameters")) {
					ImGui::MenuItem("Facial processing parameters", NULL, &show_face_processing);
					ImGui::MenuItem("Facial tracking parameters", NULL, &show_face_tracking);
					ImGui::MenuItem("Enrollment processing parameters", NULL, &show_face_enrollment);
					ImGui::MenuItem("Access control and matching", NULL, &show_control_entry_match);
					ImGui::MenuItem("Channel configuration", NULL, &show_params_channel);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			
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
			ImGui::MenuItem("Control Unidentified", NULL, &show_unidentified);
			ImGui::MenuItem("Output Control", NULL, &show_output_control);			
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void MainForm::ShowMenuConfiguration(bool* p_open)
{					
	static int hue = 140;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	
	if (!ImGui::Begin(ASSInterface::Label::GetLabel(LBL_CONFIGURATION_ALL, lg).c_str(), p_open))
	{
		ImGui::End();
		return;
	}
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(col_area.x, col_area.y, col_area.z, 0.10f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
	
	if (ImGui::TreeNode(ASSInterface::Label::GetLabel(LBL_GLOBAL_PARAMETERS, lg).c_str())) {
		if (ImGui::Button("Biometric parameters")) {
			show_params_global_biometric = true;
		}
		if (ImGui::Button("On-the-fly registration")) {
			show_params_enroll_onthefly = true;
		}
		if (ImGui::Button("Database")) {
			show_params_database = true;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Channel parameters")) {
		if (ImGui::Button("Face processing")) {
			show_face_processing = true;
		}
		if (ImGui::Button("Tracking processing")) {
			show_face_tracking = true;
		}
		if (ImGui::Button("Enrollment processing")) {
			show_face_enrollment = true;
		}
		if (ImGui::Button("Access control and matching")) {
			show_control_entry_match = true;
		}
		if (ImGui::Button("Channel configuration")) {
			show_params_channel = true;
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("License parameters")) {
		if (ImGui::Button("License data")) {

		}
		ImGui::TreePop();
	}

	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);
	ImGui::End();
}

void MainForm::ShowStatusBar() {
	std::string currentDate = ASSInterface::DateTime::Now();
	ImGuiContext& g = *GImGui;
	ImGuiViewport* viewport = g.Viewports[0];
	float height = g.FontBaseSize + g.Style.FramePadding.y;
	ImGui::SetNextWindowPos(ImVec2((viewport->Pos.x + ImVec2(0.0f, g.IO.DisplaySize.y - height).x),
		(viewport->Pos.y + ImVec2(0.0f, g.IO.DisplaySize.y - height).y)));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, height));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 0));
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings;
	ImGui::Begin("#statusbar", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	ImGui::Text(messageStatus.c_str()); ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 51)));
	ImGui::Text(progressBar.c_str(), "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]); ImGui::SameLine();
	ImGui::PopStyleColor();
	ImGui::SetCursorPos(ImVec2((viewport->Size.x - 130.0f), 0.0f));
	ImGui::Text(currentDate.c_str());
	ImGui::End();
	
}

void MainForm::ShowWindowTip(bool* p_open) {

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav ; //ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings | 
	
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Window Tip", p_open, window_flags))
	{						
		ImGui::Image((void*)txtWindowTip->GetRendererID(), ImVec2{ 60.0f, 75.0f });
		ImGui::SameLine();		
		ImGui::BeginGroup();
		ImFont* font_current = ImGui::GetFont();
		ImGui::SetWindowFontScale(0.8f);
		ImGui::PushFont(font_current);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImGui::Text("Not found");
		ImGui::Text("Observe");
		ImGui::Text("4 visits across 2 locations.");
		ImGui::Text("Last visit: 13 days ago.");
		ImGui::PopStyleColor();
		ImGui::PopFont();
		ImGui::EndGroup();
	}
	clock_t duration = clock() - timeStartWindowTip;
	int durationMs = int(1000 * ((float)duration) / CLOCKS_PER_SEC);
	if (durationMs >= 10000)
	{		
		*p_open = false;
	}

	ImGui::End();
}
