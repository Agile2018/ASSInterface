#include "MainForm.h"
#include <imgui/imgui.h>
#include "imgui/imgui_internal.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2.h>
#include "ASSInterface/Utils/PlatformUtils.h"

static bool show_configuration_menu_bar = false;
static bool show_enrollment_rav_menu_bar = false;
static bool show_enrollment_menu_bar = false;
static bool show_control_entry_menu_bar = false;
static bool show_face_processing = false;
static bool show_face_tracking = false;
static bool show_face_enrollment = false;
static bool show_control_entry_match = false;
static bool show_params_channel = false;
static bool show_params_global_biometric = false;
static bool show_params_enroll_onthefly = false;
static bool show_params_database = false;
static bool show_output_control = false;
static bool show_simple_overlay = false;
static bool show_view_reset = false;
static bool show_operational = false;
static bool show_register_debug = false;
static bool show_inside_debug = false;
static bool show_comunication_debug = false;
static bool show_mobile = false;
static bool show_params_licence = false;
static bool show_inside_control = false;
static bool show_unidentified = false;
static bool show_log = false;
static bool show_edit_register = false;
static bool show_import = false;

MainForm::MainForm() : Layer("WhoIsLayer")
{					
	InitResources();
	InitForms();

	ObserverChannelTask();

	//Controls
	*messageStatus = "Welcome";
	ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], result: [%s]\n",
		ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), 
		"Welcome, Init Log");			
}

MainForm::~MainForm()
{
	initInnovatrics->Terminate();
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
	if (videos[channel].IsCorrectStart())
	{
		if (!videos[channel].GetFlowRunning())
		{
			videos[channel].StartThread(videos[channel].GetNameThreadRun(), *p_open);
		}
		videos[channel].ShowVideo(p_open);

	}
	else
	{
		*messageStatus = "Channel disabled.";
	}
}

void MainForm::ShowMenuEnrollmentRAV(bool* p_open)
{	
	if (indexUnidentified != -1)
	{
		//frmEnrollment->SetEnabledCapture(false);
		frmEnrollment->GetPersonEnrolled()->txtCapture = frmUnidentified->GetPersonUnidentified(indexUnidentified)->txtCapture;
		frmEnrollment->GetPersonEnrolled()->bufferCapture.clear();
		frmEnrollment->GetPersonEnrolled()->bufferCapture.assign(&frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture[0], &frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture[0] + frmUnidentified->GetPersonUnidentified(indexUnidentified)->bufferCapture.size());

		std::string idTemp = std::to_string(indexUnidentified);
		strcpy(frmEnrollment->GetPersonEnrolled()->idtemp, idTemp.c_str());
		strcpy(frmEnrollment->GetPersonEnrolled()->match, frmUnidentified->GetPersonUnidentified(indexUnidentified)->match);
		strcpy(frmEnrollment->GetPersonEnrolled()->channel, frmUnidentified->GetPersonUnidentified(indexUnidentified)->channel);
		frmEnrollment->GetPersonEnrolled()->templateFace.clear();
		frmEnrollment->GetPersonEnrolled()->templateFace.assign(&frmUnidentified->GetPersonUnidentified(indexUnidentified)->templateFace[0], &frmUnidentified->GetPersonUnidentified(indexUnidentified)->templateFace[0] + frmUnidentified->GetPersonUnidentified(indexUnidentified)->templateFace.size());
		int chn = atoi(frmUnidentified->GetPersonUnidentified(indexUnidentified)->channel);
		chn -= 1;
		
		frmEnrollment->ClearTask();
		frmEnrollment->SetInnoTask(videos[chn].GetExecuteTask());

		indexUnidentified = -1;
	}

	strcpy(personSpecificationTemp.idtemp, "-1");	 
	frmEnrollment->ShowRAV(p_open, personSpecificationTemp);
	std::string idTemp = &personSpecificationTemp.idtemp[0];
	if (idTemp != "-1")
	{
		std::string state = &personSpecificationTemp.state[0];
		if (state == "Autorized")
		{
			frmOutputControl->SetPersonInside(personSpecificationTemp);
		}
		else
		{
			frmControlEntry->SetPersonDetected(personSpecificationTemp);
			
		}
		
		CloseConnections();

		frmUnidentified->RemovePersonUnidentified(atoi(idTemp.c_str()));

		//Temp
		/*int img = distribution(generator);
		std::string path = "assets/textures/" + std::to_string(img) + ".png";
		SetImageToTexture(path, txtWindowTip);
		timeStartWindowTip = clock();
		show_simple_overlay = true;
		ShowWindowTip(&show_simple_overlay);*/
		//

		*p_open = false;
	}
		
}

void MainForm::ShowMenuControlEntry(bool* p_open)
{
	
	strcpy(personSpecificationTemp.id, "-1");
	frmControlEntry->Show(p_open, personSpecificationTemp);

	std::string id = &personSpecificationTemp.id[0];
	if (id != "-1")
	{
		frmOutputControl->SetPersonInside(personSpecificationTemp);		
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
//		transImage
// 
//}

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

void MainForm::InitResources()
{
	progressBar = prgBar;

	//Language
	lg = ASSInterface::Label::GetCurrentLanguage();
	//Files
	managerFile = ASSInterface::File::Create();
	managerFile->CreateFolder(managerFile->GetFolderConfiguration());
	//Database
	dbMongo = ASSInterface::Database::Create();
	//Recognition
	initInnovatrics = ASSInterface::InitLibrary::CreateInnovatrics();	
	initInnovatrics->Init();
}

void MainForm::InitForms()
{
	//Forms
	frmConfiguration = new FrameConfiguration(lg);
	frmControlEntry = new FrameControlEntry(lg);
	frmEnrollment = new FrameEnrollment(lg);
	frmEditRegister = new FrameEditRegister(lg);
	frmImport = new FrameImport(lg);
	frmEnrollment->SetDatabase(dbMongo);
	frmConfiguration->SetDatabase(dbMongo);
	frmControlEntry->SetDatabase(dbMongo);
	frmOutputControl->SetDatabase(dbMongo);
	frmEditRegister->SetDatabase(dbMongo);
	frmImport->SetDatabase(dbMongo);
	frmImport->SetMessageStatus(messageStatus);
	frmConfiguration->SetMessageStatus(messageStatus);
	frmEnrollment->SetMessageStatus(messageStatus);
	frmEditRegister->SetMessageStatus(messageStatus);
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		videos[i].SetMessageStatus(messageStatus);
		videos[i].Init(i + 1);
		frmControlEntry->SetViewChannel(&stateVideo[i]);
		frmEnrollment->SetViewChannel(&stateVideo[i]);
		frmImport->SetViewChannel(&stateVideo[i]);
	}

	for (int i = 0; i < MAX_CHANNELS; i++) {
		videos[i].SetDatabase(dbMongo);
	}

	for (int i = 0; i < MAX_CHANNELS - 1; i++)
	{
		for (int y = 0; y < MAX_CHANNELS; y++)
		{
			frmTracking[i].SetViewChannel(&stateVideo[y]);			
		}
		frmTracking[i].SetDatabase(dbMongo);
	}

	ObserverIdentification();
	ObserverTextureDocument();
	ObserverConnection();
}

void MainForm::ShowMenuMain()
{
	
	if (show_configuration_menu_bar) ShowMenuConfiguration(&show_configuration_menu_bar);
	if (show_face_processing) frmConfiguration->ShowParametersFaceProcessing(&show_face_processing);
	if (show_face_tracking) frmConfiguration->ShowParametersFaceTracking(&show_face_tracking);
	if (show_face_enrollment) frmConfiguration->ShowParametersEnrollmentProcessing(&show_face_enrollment);
	if (show_control_entry_match) frmConfiguration->ShowParametersControlEntryMatch(&show_control_entry_match);
	if (show_params_channel) frmConfiguration->ShowParametersChannel(&show_params_channel);
	if (show_params_global_biometric) frmConfiguration->ShowParametersBiometric(&show_params_global_biometric);
	if (show_params_enroll_onthefly) frmConfiguration->ShowParametersOnthefly(&show_params_enroll_onthefly);
	if (show_params_database) frmConfiguration->ShowParametersDatabase(&show_params_database);
	if (show_params_licence) frmConfiguration->ShowParametersLicence(&show_params_licence);

	if (show_output_control) frmOutputControl->Show(&show_output_control);
	if (show_unidentified) {				
		frmUnidentified->Show(&show_unidentified,
			&show_enrollment_rav_menu_bar, indexUnidentified);
	}
	
	if (show_enrollment_menu_bar) frmEnrollment->Show(
		&show_enrollment_menu_bar, personSpecificationTemp);
	if (!show_enrollment_menu_bar) frmEnrollment->ResetChannel();

	if (show_enrollment_rav_menu_bar) ShowMenuEnrollmentRAV(&show_enrollment_rav_menu_bar);
	
	if (show_inside_control) ShowMenuControlEntry(&show_inside_control); 
	
	for (int i = 0; i < MAX_CHANNELS - 1; i++)
	{
		if (stateFrmTrack[i]) frmTracking[i].Show(&stateFrmTrack[i], i);
	}

	for (int j = 0; j < MAX_CHANNELS; j++)
	{
		if(stateVideo[j]) 
			ShowVideo(&stateVideo[j], j);
		else {
			videos[j].StopThread(videos[j].GetNameThreadRun());
			if (j + 1 == frmEnrollment->GetIndexCurrentVideo())
			{
				frmEnrollment->ResetButtonVideo();
			}
		}
			
	}

	if (!show_enrollment_rav_menu_bar) {				
		indexUnidentified = -1;
		frmEnrollment->CleanSpecification(0);		
	}
		
	if (show_simple_overlay) ShowWindowTip(&show_simple_overlay);

	if (show_log) ShowAppLog(&show_log);

	if (show_edit_register) frmEditRegister->Show(&show_edit_register);

	if (show_import) frmImport->Show(&show_import);


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("View reset", NULL, &show_view_reset);
			if (ImGui::MenuItem("Exit")) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				ASSInterface::Application::Get().Close();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Complete registration")) {
			ImGui::MenuItem("Register", NULL, &show_enrollment_menu_bar);
			ImGui::MenuItem("Edit Register", NULL, &show_edit_register);
			ImGui::MenuItem("Imports", NULL, &show_import);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Reports")) {

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Entry control")) {

			if (ImGui::BeginMenu("Operational")) {
				ImGui::MenuItem("Inside control", NULL, &show_inside_control);
				ImGui::MenuItem("Outside control", NULL, &show_output_control);
				ImGui::MenuItem("Unidentified control", NULL, &show_unidentified);
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Deployment")) {
				
				for (int i = 0; i < MAX_CHANNELS - 1; i++)
				{
					std::string name = "Tracking control " + std::to_string(i);
					ImGui::MenuItem(name.c_str(), NULL, &stateFrmTrack[i]);
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMenu();
		}

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

			if (ImGui::BeginMenu("Client parameters")) {
				ImGui::MenuItem("Data licence", NULL, &show_params_licence);
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Register", NULL, &show_register_debug);
			ImGui::MenuItem("Inside", NULL, &show_inside_debug);
			ImGui::MenuItem("Comunication", NULL, &show_comunication_debug);
			ImGui::MenuItem("Mobile", NULL, &show_mobile);
			ImGui::MenuItem("Log", NULL, &show_log);
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
	if (ImGui::TreeNode("Client parameters")) {
		if (ImGui::Button("License data")) {
			show_params_licence = true;
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
	ImGui::Text((*messageStatus).c_str()); ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 51)));
	ImGui::Text(progressBar.c_str(), "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]); ImGui::SameLine();
	ImGui::PopStyleColor();
	ImGui::SetCursorPos(ImVec2((viewport->Size.x - 500.0f), 0.0f));
	ImGui::Text("Operator:");
	ImGui::SetCursorPos(ImVec2((viewport->Size.x - 420.0f), 0.0f));
	ImGui::Text(nameOperator.c_str());
	ImGui::SetCursorPos(ImVec2((viewport->Size.x - 130.0f), 0.0f));
	ImGui::Text(currentDate.c_str());

	ImGui::End();
	
}

void MainForm::ShowWindowTip(bool* p_open) {

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav ; //ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings | 
	
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Window Tip", p_open, window_flags))
	{						
		//ImGui::Image((void*)txtWindowTip->GetRendererID(), ImVec2{ 60.0f, 75.0f });
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

void MainForm::ObserverChannelTask()
{
	auto channelObservable = frmControlEntry->observableCurrentChannel
		.map([](int channelSelect) {
			return channelSelect;
		});

	auto subscriptionChannel = channelObservable
		.subscribe([this](int channelSelect) {		
			if (videos[channelSelect].IsCorrectStart()) {
				videos[channelSelect].SetTask(CONTROL_ENTRY);
			}				
		});

	subscriptionChannel.clear();

	auto channelObservableEnroll = frmEnrollment->GetChannel()
		.map([](int channelSelect) {
			return channelSelect;
		});

	auto subscriptionChannelEnroll = channelObservableEnroll
		.subscribe([this](int channelSelect) {
			if (videos[channelSelect].IsCorrectStart()) {
				videos[channelSelect].SetTask(ENROLL_PERSON);
				
			}
		});

	subscriptionChannelEnroll.clear();

	std::vector<rxcpp::observable<std::tuple<int, int>>> observersChannel;
	for (int i = 0; i < MAX_CHANNELS - 1; i++)
	{
		observersChannel.push_back(frmTracking[i].GetChannel()
			.map([](std::tuple<int, int> tupleChannel) {
				return tupleChannel;
			}));
	}

	std::vector<rxcpp::subscription> subscriptionsChannel;

	for (int i = 0; i < MAX_CHANNELS - 1; i++)
	{
		subscriptionsChannel.push_back(observersChannel[i]
			.subscribe([this](std::tuple<int, int> tupleChannel) {
				
				switch (std::get<1>(tupleChannel))
				{
				case 0:
				{
					int chn = std::get<0>(tupleChannel);
					if (videos[chn].IsCorrectStart()) {
						videos[chn].SetTask(CONTROL_ENTRY_TRACK0);
					}
					
					break;
				}					
				case 1:
				{
					int chn = std::get<0>(tupleChannel);
					if (videos[chn].IsCorrectStart()) {
						videos[chn].SetTask(CONTROL_ENTRY_TRACK1);
					}
					break;
				}					
				default:
					break;
				}

			}));

	}
	
}

void MainForm::ObserverIdentification()
{
	std::vector<rxcpp::observable<PersonSpecification>> observersPersons;
	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		observersPersons.push_back(videos[i].GetDataPerson()
			.map([](PersonSpecification personSpecification) {
				return personSpecification;
			}));
	}

	std::vector<rxcpp::subscription> subscriptionsPersons;

	for (int i = 0; i < MAX_CHANNELS; i++)
	{
		subscriptionsPersons.push_back(observersPersons[i]
			.subscribe([this](PersonSpecification personSpecification) {
				DefinePipe(personSpecification);
			}));

	}	
}

void MainForm::ObserverTextureDocument()
{
	auto documentObservableObverse = frmEnrollment->GetTextDocumentObverse()
		.map([](int channelSelect) {
			return channelSelect;
		});

	auto subscriptionDocumentObverse = documentObservableObverse
		.subscribe([this](int channelSelect) {
			frmEnrollment->SetDocumentObverse(videos[channelSelect].GetDataTexture(),
				videos[channelSelect].GetWidthImage(), videos[channelSelect].GetHeightImage());
		});

	subscriptionDocumentObverse.clear();

	auto documentObservableReverse = frmEnrollment->GetTextDocumentReverse()
		.map([](int channelSelect) {
		return channelSelect;
			});

	auto subscriptionDocumentReverse = documentObservableReverse
		.subscribe([this](int channelSelect) {
		frmEnrollment->SetDocumentReverse(videos[channelSelect].GetDataTexture(),
			videos[channelSelect].GetWidthImage(), videos[channelSelect].GetHeightImage());
		});

	subscriptionDocumentReverse.clear();
}

void MainForm::ObserverConnection()
{
	auto connectedObservable = frmEnrollment->CloseConnection()
		.map([](bool close) {
			return close;
		});

	auto subscriptionConnection = connectedObservable
		.subscribe([this](bool close) {
			if (close)
			{
				CloseConnections();
			}
		});

	subscriptionConnection.clear();
}

void MainForm::DefinePipe(PersonSpecification specPerson)
{	
	int channel = atoi(specPerson.channel);
	channel -= 1;

	int id = atoi(specPerson.id);
	if (id == 0 && specPerson.task != ENROLL_PERSON)
	{		
		frmUnidentified->SetPersonUnidentified(specPerson);
	}
	else
	{				
		switch (videos[channel].GetTask())
		{
		case CONTROL_ENTRY:
			frmControlEntry->SetPersonTemp(specPerson);
			break;
		case CONTROL_ENTRY_TRACK0:
			frmTracking[0].SetPersonTemp(specPerson);
			break;
		case CONTROL_ENTRY_TRACK1:
			frmTracking[1].SetPersonTemp(specPerson);
			break;		
		default:
			break;
		}
		
		if (specPerson.task == ENROLL_PERSON)
		{
			frmEnrollment->SetDataEnrollTemp(specPerson);
		}
	}
}

void MainForm::CloseConnections()
{
	for (int i = 0; i < MAX_CHANNELS; i++) {
		if (videos[i].GetTask() == CONTROL_ENTRY_TRACK0 || 
			videos[i].GetTask() == CONTROL_ENTRY_TRACK1 || 
			videos[i].GetTask() == CONTROL_ENTRY)
		{
			videos[i].CloseConnection();
		}

	}
}
void MainForm::ShowAppLog(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
	ASSInterface::ImGuiLog::Draw("WhoIs: Log", p_open);
}