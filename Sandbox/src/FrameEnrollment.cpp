#include "hzpch.h"
#include "FrameEnrollment.h"
#include "ASSInterface/Utils/PlatformUtils.h"
#include "imgui/imgui.h"
#include <SOIL2.h>

FrameEnrollment::FrameEnrollment(ASSInterface::LanguageType language)
{
	lg = language;
	base64 = ASSInterface::Base64::Create();	
	transImage = ASSInterface::TransformImage::Create();	
	txtButtonCapture = ASSInterface::Texture2D::Create("assets/textures/camera.png");
	txtButtonDelete = ASSInterface::Texture2D::Create("assets/textures/delete.png");	
}

FrameEnrollment::~FrameEnrollment()
{
	if (innoTask != nullptr)
	{
		innoTask->CloseConnection();
		innoTask = nullptr;
		innoDetect = nullptr;
	}
}

void FrameEnrollment::Show(bool* p_open, PersonSpecification& personSpec)
{
	isEnrollRAV = false;
	static const char* currentItemPersonType = NULL;
	static const char* current_video = NULL;
	ImTextureID tidCapture = (ImTextureID)txtButtonCapture->GetRendererID();
	ImTextureID tidDelete = (ImTextureID)txtButtonDelete->GetRendererID();	
	std::string dateEnroll = ASSInterface::DateTime::Now();
	strcpy(personSpecificationEnrollComplete.date, dateEnroll.c_str());
	ImGuiTabItemFlags_ flagTab1 = ImGuiTabItemFlags_None;
	ImGuiTabItemFlags_ flagTab2 = ImGuiTabItemFlags_None;
	ImGuiTabItemFlags_ flagTab3 = ImGuiTabItemFlags_None;
	ImGuiTabItemFlags_ flagTab4 = ImGuiTabItemFlags_None;
	static bool popup_open = false;
	static bool popup_open_doc = false;
	static bool popup_file_open = false;
	/*if (currentIndexVideo == 0 && !*viewVideo[0]) currentIndexVideo = -1;
	if (currentIndexVideo == 1 && !*viewVideo[1]) currentIndexVideo = -1;*/

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment", p_open)) 
	{
		ImGui::End();
		return;
	}
	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	if (ImGui::BeginTabBar("Enroll overall"))
	{
		if (ImGui::BeginTabItem("Personal information", 0, flagTab1))
		{
			
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.5f, 60.0f));
			ImGui::Text("First Name:"); ImGui::SameLine();
			ImGui::PushItemWidth(200);
			ImGui::InputText("##name", personSpecificationEnrollComplete.name, IM_ARRAYSIZE(personSpecificationEnrollComplete.name));

			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.5f, 90.0f));
			ImGui::Text("Last Name:"); ImGui::SameLine();
			ImGui::PushItemWidth(200);
			ImGui::InputText("##lastname", personSpecificationEnrollComplete.lastname, IM_ARRAYSIZE(personSpecificationEnrollComplete.lastname));

			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.5f, 120.0f));
			ImGui::Text("C.I.:"); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::InputText("##document", personSpecificationEnrollComplete.document, IM_ARRAYSIZE(personSpecificationEnrollComplete.document), ImGuiInputTextFlags_CharsNoBlank);
			
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.5f, 150.0f));
			ImGui::Text("Type Person:"); ImGui::SameLine();
			if (ImGui::BeginCombo("##combotypeperson", currentItemPersonType)) {
				for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::TYPES_PERSON); n++) {
					bool is_selected = (currentItemPersonType == ConstantApplication::TYPES_PERSON[n]);
					if (ImGui::Selectable(ConstantApplication::TYPES_PERSON[n], is_selected)) {
						currentItemPersonType = ConstantApplication::TYPES_PERSON[n];
						strcpy(personSpecificationEnrollComplete.type, currentItemPersonType);
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();
			
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 100.0f), 190.0f));
			if (ImGui::Button("Next...")) {				
				flagTab2 = ImGuiTabItemFlags_SetSelected;
				
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Capture Images", 0, flagTab2)) {
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.5f, 60.0f));
			SetTextureNewDetected();
			ImGui::Image(reinterpret_cast<void*>(intptr_t(personSpecificationEnrollComplete.txtCapture)),
				ImVec2{ 120.0f, 149.0f });
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text(ASSInterface::Label::GetLabel(LBL_NAME, lg).c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.name);
			ImGui::Text(ASSInterface::Label::GetLabel(LBL_LAST_NAME, lg).c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.lastname);
			ImGui::Text("C.I.:"); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.document);
			ImGui::Text("Type Person:"); ImGui::SameLine();
			ImGui::Text(personSpecificationEnrollComplete.type);
			ImGui::EndGroup();

			ImGui::Separator();
			
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 200.0f) * 0.5f, 220.0f));
			if (ImGui::Button("File")) {
				if (!isVideoButton)
				{
					
					if (currentIndexVideo == -1)
					{
						popup_file_open = true;
						ImGui::SetNextWindowPos(ImGui::GetMousePos());

					}
					else
					{
						OpenFile();
					}

				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Camera")) {
				popup_open = true;
				ImGui::SetNextWindowPos(ImGui::GetMousePos());
				
			}

			if (popup_open)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::Begin("##Popup", &popup_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				if (!ImGui::IsWindowFocused())
					popup_open = false;

				for (int i = 0; i < IM_ARRAYSIZE(ConstantApplication::QUANTITY_CHANNELS); i++) {

					bool is_selected = (currentChannel == ConstantApplication::QUANTITY_CHANNELS[i]);

					if (ImGui::Selectable(ConstantApplication::QUANTITY_CHANNELS[i], is_selected))
					{
						currentChannel = ConstantApplication::QUANTITY_CHANNELS[i];
						ImGui::SetItemDefaultFocus();
						if (!*viewVideo[i])
						{
							*messageStatus = "";
							*viewVideo[i] = true;
							currentIndexVideo = i + 1;
							ClearTask();
							isVideoButton = true;							
							
						}
						else
						{
							*messageStatus = "The channel is busy.";
						}
						popup_open = false;
					}

				}

				ImGui::End();
				ImGui::PopStyleVar();

			}

			if (popup_file_open)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::Begin("##Popupfile", &popup_file_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				if (!ImGui::IsWindowFocused())
					popup_file_open = false;

				for (int i = 0; i < IM_ARRAYSIZE(ConstantApplication::QUANTITY_CHANNELS); i++) {

					bool is_selected = (currentChannel == ConstantApplication::QUANTITY_CHANNELS[i]);

					if (ImGui::Selectable(ConstantApplication::QUANTITY_CHANNELS[i], is_selected))
					{
						currentChannel = ConstantApplication::QUANTITY_CHANNELS[i];
						ImGui::SetItemDefaultFocus();
						if (!*viewVideo[i])
						{							
							*messageStatus = "";
							currentIndexVideo = i + 1;
							//isFileButton = true;
							//ClearTask();
							shootChannel.on_next(currentIndexVideo - 1);
							CreateTaskEnroll(currentIndexVideo);
							OpenFile();

						}
						else
						{
							*messageStatus = "The channel is busy.";													
						}

						popup_file_open = false;
					}

				}

				ImGui::End();
				ImGui::PopStyleVar();

			}

			if (isVideoButton)
			{
				ImGui::SameLine();

				if (ImGui::ImageButton(tidCapture, ImVec2(24, 24))) {
					shootChannel.on_next(currentIndexVideo - 1);

				}
				ImGui::SameLine();
				if (ImGui::ImageButton(tidDelete, ImVec2(24, 24))) {
					listDetected.pop_back();
					personSpecificationEnrollComplete.txtCapture = 0;
				}
				
			}
			
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 100.0f), 220.0f));
			if (ImGui::Button("Next...")) {
				flagTab3 = ImGuiTabItemFlags_SetSelected;
			}

			ImGui::Separator();

			ImGui::BeginChild("Templates", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Columns(5);

			for (size_t i = 0; i < listDetected.size(); i++)
			{
				ImGui::Image(reinterpret_cast<void*>(intptr_t(listDetected[i].txtCapture)),
					ImVec2{ 80.0f, 100.0f });
				ImGui::NextColumn();
			}


			ImGui::Columns();

			ImGui::EndChild();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Load Document", 0, flagTab3)) {
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 100.0f) * 0.25f, 60.0f));
			ImGui::Text("Obverse"); 
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 100.0f) * 0.75f, 60.0f));
			ImGui::Text("Reverse"); 
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 420.0f) * 0.5f, 90.0f));
			SetTextureDocument();
			
			if (ImGui::ImageButton((void*)personSpecificationEnrollComplete.txtDocumentObverse,
				ImVec2{ 200.0f, 115.0f })) {
				personSpecificationEnrollComplete.txtDocumentObverse = 0;
				if (currentIndexVideoDoc != -1)
				{
					shootRequireObverse.on_next(currentIndexVideoDoc - 1);
				}
				else
				{
					*messageStatus = "Select channel.";
				}
				
			}
			
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)personSpecificationEnrollComplete.txtDocumentReverse,
				ImVec2{ 200.0f, 115.0f })) {
				personSpecificationEnrollComplete.txtDocumentReverse = 0;
				if (currentIndexVideoDoc != -1)
				{
					shootRequireReverse.on_next(currentIndexVideoDoc - 1);
				}
				else
				{
					*messageStatus = "Select channel.";
				}
				
			}

			ImGui::Spacing();

			ImGui::Text("Document face:"); ImGui::SameLine();
			ImGui::RadioButton("Obverse", &docFace, 1); ImGui::SameLine();
			ImGui::RadioButton("Reverse", &docFace, 2); ImGui::SameLine();
			if (ImGui::Button("File")) {
				OpenFileDocument();
			}
			ImGui::SameLine();
			if (ImGui::Button("Camera")) {
				popup_open_doc = true;
				ImGui::SetNextWindowPos(ImGui::GetMousePos());
			}

			if (popup_open_doc)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::Begin("##Popupdoc", &popup_open_doc, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

				if (!ImGui::IsWindowFocused())
					popup_open_doc = false;

				for (int i = 0; i < IM_ARRAYSIZE(ConstantApplication::QUANTITY_CHANNELS); i++) {

					bool is_selected = (currentChannelDoc == ConstantApplication::QUANTITY_CHANNELS[i]);

					if (ImGui::Selectable(ConstantApplication::QUANTITY_CHANNELS[i], is_selected))
					{
						currentChannelDoc = ConstantApplication::QUANTITY_CHANNELS[i];
						ImGui::SetItemDefaultFocus();
						if (!*viewVideo[i])
						{
							*messageStatus = "";
							*viewVideo[i] = true;
							currentIndexVideoDoc = i + 1;
							ClearTask();
							
						}
						else
						{
							*messageStatus = "The channel is busy.";
						}
						popup_open_doc = false;
					}

				}

				ImGui::End();
				ImGui::PopStyleVar();

			}


			ImGui::Separator();
			ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 100.0f), 260.0f));
			if (ImGui::Button("Next...")) {
				flagTab4 = ImGuiTabItemFlags_SetSelected;
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Record Summary", 0, flagTab4)) {
			ImGui::BeginGroup();

			ImGui::ImageButton((void*)personSpecificationEnrollComplete.txtDocumentObverse, ImVec2{ 200.0f, 115.0f });
			ImGui::ImageButton((void*)personSpecificationEnrollComplete.txtDocumentReverse, ImVec2{ 200.0f, 115.0f });
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::Image((void*)personSpecificationEnrollComplete.txtCapture,
				ImVec2{ 120.0f, 149.0f });			
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text(ASSInterface::Label::GetLabel(LBL_NAME, lg).c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.name);
			ImGui::Text(ASSInterface::Label::GetLabel(LBL_LAST_NAME, lg).c_str()); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.lastname);
			ImGui::Text("C.I.:"); ImGui::SameLine();
			ImGui::PushItemWidth(150);
			ImGui::Text(personSpecificationEnrollComplete.document);
			ImGui::Text("Type Person:"); ImGui::SameLine();
			ImGui::Text(personSpecificationEnrollComplete.type);
			ImGui::EndGroup();
			ImGui::Separator();
			ImGui::Spacing();
			if (ImGui::Button("Save")) {
				if (SavePerson()) {
					CleanSpecification();
					shootCloseConnection.on_next(true);
				}
			}
			ImGui::EndTabItem();
		}
		

		ImGui::EndTabBar();
	}	

	ImGui::End();
}

void FrameEnrollment::ShowRAV(bool* p_open, PersonSpecification& personSpec)
{
	static const char* current_place = NULL;
	static bool popup_open_doc = false;
	std::string dateEnroll = ASSInterface::DateTime::Now();
	strcpy(personSpecificationEnroll.date, dateEnroll.c_str());
	isEnrollRAV = true;
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment On The Fly (Mode: Individual)", p_open))
	{
		ImGui::End();
		return;
	}

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 120.0f) * 0.5f, 30.0f));
	ImGui::Image((void*)personSpecificationEnroll.txtCapture, ImVec2{ 120.0f, 149.0f });

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("First Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##name", personSpecificationEnroll.name, IM_ARRAYSIZE(personSpecificationEnroll.name));
	ImGui::Text("Last Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##lastname", personSpecificationEnroll.lastname, IM_ARRAYSIZE(personSpecificationEnroll.lastname));
	ImGui::Text("Document:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##document", personSpecificationEnroll.document, IM_ARRAYSIZE(personSpecificationEnroll.document), ImGuiInputTextFlags_CharsNoBlank);
	ImGui::Text("Visitor Gallery");
	strcpy(personSpecificationEnroll.type, ConstantApplication::TYPES_PERSON[0]);
	ImGui::Spacing();
	ImGui::Separator();

	SetTextureDocument();

	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 200.0f) * 0.5f, 315.0f));	
	if (ImGui::ImageButton((void*)personSpecificationEnroll.txtDocumentObverse, 
		ImVec2{ 200.0f, 115.0f })) {
		personSpecificationEnroll.txtDocumentObverse = 0;	
		if (currentIndexVideoDoc != -1)
		{
			shootRequireObverse.on_next(currentIndexVideoDoc - 1);
		}
		else
		{
			*messageStatus = "Select channel.";
		}
		
	}
	ImGui::Spacing();
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 200.0f) * 0.5f, 445.0f));
	if (ImGui::ImageButton((void*)personSpecificationEnroll.txtDocumentReverse, 
		ImVec2{ 200.0f, 115.0f })) {		
		personSpecificationEnroll.txtDocumentReverse = 0;
		if (currentIndexVideoDoc != -1)
		{
			shootRequireReverse.on_next(currentIndexVideoDoc - 1);
		}
		else
		{
			*messageStatus = "Select channel.";
		}
		
	}

	ImGui::Spacing();

	ImGui::Text("Document face:"); ImGui::SameLine();
	ImGui::RadioButton("Obverse", &docFace, 1); ImGui::SameLine();
	ImGui::RadioButton("Reverse", &docFace, 2); ImGui::SameLine();
	if (ImGui::Button("File")) {
		OpenFileDocument();
	}
	ImGui::SameLine();
	if (ImGui::Button("Camera")) {
		popup_open_doc = true;
		ImGui::SetNextWindowPos(ImGui::GetMousePos());
	}

	if (popup_open_doc)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("##Popupdoc", &popup_open_doc, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (!ImGui::IsWindowFocused())
			popup_open_doc = false;

		for (int i = 0; i < IM_ARRAYSIZE(ConstantApplication::QUANTITY_CHANNELS); i++) {

			bool is_selected = (currentChannelDoc == ConstantApplication::QUANTITY_CHANNELS[i]);

			if (ImGui::Selectable(ConstantApplication::QUANTITY_CHANNELS[i], is_selected))
			{
				currentChannelDoc = ConstantApplication::QUANTITY_CHANNELS[i];
				ImGui::SetItemDefaultFocus();
				if (!*viewVideo[i])
				{
					*messageStatus = "";
					*viewVideo[i] = true;
					currentIndexVideoDoc = i + 1;
					//ClearTask();

				}
				else
				{
					*messageStatus = "The channel is busy.";
				}
				popup_open_doc = false;
			}

		}

		ImGui::End();
		ImGui::PopStyleVar();

	}


	ImGui::Spacing();
	ImGui::Separator();

	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 150.0f) * 0.5f, 610.0f));
	if (ImGui::Button("Save Register", ImVec2(150.0f, 25.0f))) {
		std::string autorized = "Not autorized";
		strcpy(personSpecificationEnroll.state, autorized.c_str());
		if (SavePerson(personSpec)) SaveEventRegister(personSpec);
	}
	
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 243.0f) * 0.5f , 640.0f));
	ImGui::Text("Place:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combo", personSpecificationEnroll.place)) {
		for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::DESTINATION_PLACES); n++) {
			bool is_selected = (current_place == ConstantApplication::DESTINATION_PLACES[n]);
			if (ImGui::Selectable(ConstantApplication::DESTINATION_PLACES[n], is_selected)) {
				current_place = ConstantApplication::DESTINATION_PLACES[n];
				strcpy(personSpecificationEnroll.place, current_place);
				ImGui::SetItemDefaultFocus(); 
			}
		}
		ImGui::EndCombo();
	}
	
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 150.0f) * 0.5f, 670.0f));
	if (ImGui::Button("Authorize entry", ImVec2(150.0f, 25.0f))) {
		std::string autorized = "Autorized";
		strcpy(personSpecificationEnroll.state, autorized.c_str());
		if (SavePerson(personSpec)) {
			SaveEventRegister(personSpec);
			SaveEventInside(personSpec);
		}
	}

	ImGui::End();
}

bool FrameEnrollment::SavePerson(PersonSpecification& personSpec) {		
	ASSInterface::EntitySpecification entSpec;
	entSpec.name = &personSpecificationEnroll.name[0];
	entSpec.lastName = &personSpecificationEnroll.lastname[0];
	entSpec.identification = &personSpecificationEnroll.document[0];
	entSpec.date = ASSInterface::DateTime::NowToLong();
	entSpec.type = &personSpecificationEnroll.type[0];

	if (!entSpec.name.empty() && !entSpec.identification.empty() && 
		!entSpec.type.empty() && !personSpecificationEnroll.bufferCapture.empty() && 
		!entSpec.lastName.empty() && !personSpecificationEnroll.templateFace.empty())
	{
		
		ASSInterface::TemplateSpecification specTemplate;
		specTemplate.templateData.assign(&personSpecificationEnroll.templateFace[0],
			&personSpecificationEnroll.templateFace[0] + personSpecificationEnroll.templateFace.size());
		specTemplate.faceSerialized.assign(&personSpecificationEnroll.faceSerialized[0],
			&personSpecificationEnroll.faceSerialized[0] + personSpecificationEnroll.faceSerialized.size());

		int id;
		innoTask->Enroll(specTemplate, &id);

		if (id > 0) {
			std::string strId = std::to_string(id);
			strcpy(personSpecificationEnroll.id, strId.c_str());
			entSpec.id = &personSpecificationEnroll.id[0];
			entSpec.dataImage = base64->base64_encode(&personSpecificationEnroll.bufferCapture[0], personSpecificationEnroll.bufferCapture.size());

			if (!personSpecificationEnroll.bufferDocumentObverse.empty())
			{
				entSpec.dataDocObverse = base64->base64_encode(&personSpecificationEnroll.bufferDocumentObverse[0], personSpecificationEnroll.bufferDocumentObverse.size());

			}

			if (!personSpecificationEnroll.bufferDocumentReverse.empty())
			{
				entSpec.dataDocReverse = base64->base64_encode(&personSpecificationEnroll.bufferDocumentReverse[0], personSpecificationEnroll.bufferDocumentReverse.size());

			}

			dbMongo->Add(entSpec);

			strcpy(personSpec.id, personSpecificationEnroll.id);
			strcpy(personSpec.idtemp, personSpecificationEnroll.idtemp);
			strcpy(personSpec.name, personSpecificationEnroll.name);
			strcpy(personSpec.lastname, personSpecificationEnroll.lastname);
			strcpy(personSpec.document, personSpecificationEnroll.document);
			strcpy(personSpec.date, personSpecificationEnroll.date);
			strcpy(personSpec.type, personSpecificationEnroll.type);
			strcpy(personSpec.place, personSpecificationEnroll.place);
			strcpy(personSpec.state, personSpecificationEnroll.state);
			strcpy(personSpec.match, personSpecificationEnroll.match);
			strcpy(personSpec.channel, personSpecificationEnroll.channel);

			personSpec.txtCapture = personSpecificationEnroll.txtCapture;
			personSpec.txtGallery = personSpecificationEnroll.txtCapture;
			personSpec.bufferCapture.clear();

			personSpec.bufferDocumentObverse.clear();

			personSpec.bufferDocumentReverse.clear();

			return true;
		}
		else
		{
			*messageStatus = "Enrollment Fail.";
		}		
	}
	else
	{
		*messageStatus = "Empty Fields";
	}
		
	return false;
}

bool FrameEnrollment::SavePerson()
{
	std::string strId = &personSpecificationEnrollComplete.id[0];
	int id = atoi(strId.c_str());
	if (id == 0)
	{
		if (SaveFaces())
		{
			return true;
		}
	}
	else
	{
		if (SaveDatabase()) {			
			return true;
		}
	}
	
	return false;
}

void FrameEnrollment::SaveEventRegister(PersonSpecification& personSpec)
{
	ASSInterface::EntityEvent entEvent;
	entEvent.id = &personSpec.id[0];
	entEvent.date = ASSInterface::DateTime::NowToLong();
	entEvent.type = ASSInterface::EventAppType::enrollmentOnTheFly;
	entEvent.description = "Register person Unidentified.";

	dbMongo->Add(entEvent);
	currentIndexVideoDoc = -1;
}

void FrameEnrollment::SaveEventInside(PersonSpecification& personSpec)
{
	ASSInterface::EntityEvent entEvent;
	entEvent.id = &personSpec.id[0];
	entEvent.date = ASSInterface::DateTime::NowToLong();
	entEvent.type = ASSInterface::EventAppType::inside;
	entEvent.description = &personSpec.place[0];

	dbMongo->Add(entEvent);
}

void FrameEnrollment::CreateTaskEnroll(int channel)
{
	//innoTask = ASSInterface::ExecuteTask::CreateInnoTask(channel);
	innoDetect = ASSInterface::Detection::CreateInnoDetected(channel);	
	ObserverDetected();	
}

void FrameEnrollment::ObserverEnroll()
{
	auto taskObservable = innoTask->Get()
		.map([](ASSInterface::IdentitySpecification specIdentify) {
			return specIdentify;
		});

	auto subscriptionTask = taskObservable
		.subscribe([this](ASSInterface::IdentitySpecification specIdentify) {

			if (!specIdentify.cropData.empty() && 
				specIdentify.task == ENROLL_WITH_TEMPLATES)
			{
				SetDataEnrollFace(specIdentify);
			}

		});
	subscriptionTask.clear();	
}

void FrameEnrollment::SetDataEnrollFace(ASSInterface::IdentitySpecification specIdentify)
{
	PersonSpecification personSpecification;

	personSpecification.bufferCapture.assign(&specIdentify.cropData[0],
		&specIdentify.cropData[0] + specIdentify.cropLength);

	personSpecification.cropWidth = specIdentify.cropWidth;
	personSpecification.cropHeight = specIdentify.cropHeight;
	personSpecification.task = specIdentify.task;
	
	strcpy(personSpecification.match, std::to_string(specIdentify.match).c_str());
	strcpy(personSpecification.channel, std::to_string(specIdentify.channel).c_str());

	int sizeTemplate = specIdentify.size;
	if (sizeTemplate != 0)
	{
		personSpecification.size = sizeTemplate;
		personSpecification.templateFace.assign(&specIdentify.templateData[0],
			&specIdentify.templateData[0] + sizeTemplate);
	}

	personSpecification.txtCapture = 0;
	personSpecification.txtCapture = SOIL_load_OGL_texture_from_memory(
		&specIdentify.cropData[0], (int)specIdentify.cropData.size(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (personSpecificationEnrollComplete.txtCapture == 0)
	{
		personSpecificationEnrollComplete.txtCapture = personSpecification.txtCapture;

	}

	listDetected.push_back(personSpecification);
}

void FrameEnrollment::ObserverDetected()
{
	auto taskDetectedObservable = innoDetect->Get()
		.map([](std::vector<ASSInterface::DetectSpecification> specDetections) {
			return specDetections;
		});

	auto subscriptionTaskDetect = taskDetectedObservable
		.subscribe([this](std::vector<ASSInterface::DetectSpecification> specDetections) {
			innoTask->DoTask(specDetections);
		});

	subscriptionTaskDetect.clear();
}

void FrameEnrollment::SetTextureNewDetected()
{
	if (!listDetected.empty() && listDetected.back().txtCapture == 0) {
		listDetected.back().txtCapture = SOIL_load_OGL_texture_from_memory(
			&listDetected.back().bufferCapture[0],
			(int)listDetected.back().bufferCapture.size(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		if (listDetected.back().txtCapture != 0) {
			personSpecificationEnrollComplete.txtCapture = 0;
			personSpecificationEnrollComplete.txtCapture = listDetected.back().txtCapture;
		}
	}
	
}

bool FrameEnrollment::SaveDatabase()
{
	ASSInterface::EntitySpecification entSpec;
	entSpec.id = &personSpecificationEnrollComplete.id[0];
	entSpec.name = &personSpecificationEnrollComplete.name[0];
	entSpec.lastName = &personSpecificationEnrollComplete.lastname[0];
	entSpec.identification = &personSpecificationEnrollComplete.document[0];
	entSpec.date = ASSInterface::DateTime::NowToLong();
	entSpec.type = &personSpecificationEnrollComplete.type[0];

	if (!entSpec.name.empty() && !entSpec.identification.empty() &&
		!entSpec.type.empty() && !personSpecificationEnrollComplete.bufferCapture.empty() &&
		!entSpec.lastName.empty() && !entSpec.identification.empty()) {

		entSpec.dataImage = base64->base64_encode(&personSpecificationEnrollComplete.bufferCapture[0], personSpecificationEnrollComplete.bufferCapture.size());

		if (!personSpecificationEnrollComplete.bufferDocumentObverse.empty())
		{
			entSpec.dataDocObverse = base64->base64_encode(&personSpecificationEnrollComplete.bufferDocumentObverse[0], personSpecificationEnrollComplete.bufferDocumentObverse.size());
		}
		
		if (!personSpecificationEnrollComplete.bufferDocumentReverse.empty())
		{
			entSpec.dataDocReverse = base64->base64_encode(&personSpecificationEnrollComplete.bufferDocumentReverse[0], personSpecificationEnrollComplete.bufferDocumentReverse.size());
		}
		

		dbMongo->Add(entSpec);

		ASSInterface::EntityEvent entEvent;
		entEvent.id = &personSpecificationEnrollComplete.id[0];
		entEvent.date = ASSInterface::DateTime::NowToLong();
		entEvent.type = ASSInterface::EventAppType::enrollment;
		entEvent.description = "Enroll Person.";

		dbMongo->Add(entEvent);

		*messageStatus = "Save person OK.";

		return true;
	}
	else
	{
		*messageStatus = "Empty Fields";

		return false;
	}
}

bool FrameEnrollment::SaveFaces()
{	
	if (!listDetected.empty())
	{
		std::vector<ASSInterface::TemplateSpecification> specTemplates;

		innoTask = ASSInterface::ExecuteTask::CreateInnoTask(currentIndexVideo);

		for (int i = 0; i < listDetected.size(); i++)
		{
			ASSInterface::TemplateSpecification spec;
			spec.templateData.assign(&listDetected[i].templateFace[0],
				&listDetected[i].templateFace[0] + listDetected[i].templateFace.size());
			spec.size = listDetected[i].size;
			std::string strQuality = &listDetected[i].match[0];
			spec.quality = atoi(strQuality.c_str());
			if (!listDetected[i].faceSerialized.empty())
			{
				spec.faceSerialized.assign(&listDetected[i].faceSerialized[0],
					&listDetected[i].faceSerialized[0] + listDetected[i].faceSerialized.size());
			}
			specTemplates.push_back(spec);
		}

		int id = 0;

		innoTask->EnrollTemplates(specTemplates, &id);
		specTemplates.clear();

		if (id != 0)
		{
			std::string strId = std::to_string(id);
			strcpy(personSpecificationEnrollComplete.id, strId.c_str());
			personSpecificationEnrollComplete.bufferCapture.assign(&listDetected[0].bufferCapture[0],
				&listDetected[0].bufferCapture[0] + (int)listDetected[0].bufferCapture.size());
			if (SaveDatabase()) {				
				*messageStatus = "Save person OK.";
				innoTask = nullptr;
				return true;
			}
		}
		innoTask = nullptr;
	}
	*messageStatus = "Error operation save.";
	return false;
}

void FrameEnrollment::OpenFile()
{
	std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile(
		"Images (*.png)\0*.png\0(*.jpg)\0*.jpg\0");
	if (!filePaths.empty())
	{
		listDetected.clear();
		personSpecificationEnrollComplete.txtCapture = 0;
		innoDetect->BuildTemplatesFromPersonFiles(filePaths);
		
	}
}

void FrameEnrollment::OpenFileDocument()
{
	int width, height, channels, lenght, size;

	std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile(
		"Images (*.png)\0*.png\0(*.jpg)\0*.jpg\0");

	if (!filePaths.empty() && transImage->GetSize(filePaths[0].c_str(), &width, &height))
	{		
		
		unsigned char* rawImage = NULL;

		if (width > MAX_SIZE_IMAGE) {
			unsigned char* dataImg = SOIL_load_image(filePaths[0].c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
			rawImage = transImage->Resize(dataImg, &width, &height, &lenght);
			size = lenght;
		}
		else
		{
			rawImage = SOIL_load_image(filePaths[0].c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
			size = width * height * channels;
		}

		
		std::vector<unsigned char> imgTemp;
		imgTemp.resize(200 * MB);
		transImage->CaptureDocument(rawImage, &width, &height, imgTemp);
		
		
		switch (docFace)
		{
		case 1:
		{					
			if (isEnrollRAV)
			{				
				personSpecificationEnroll.bufferDocumentObverse.clear();
				std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
					std::back_inserter(personSpecificationEnroll.bufferDocumentObverse));
				
				personSpecificationEnroll.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
					&personSpecificationEnroll.bufferDocumentObverse[0],
					(int)personSpecificationEnroll.bufferDocumentObverse.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
					

			}
			else
			{
				
				personSpecificationEnrollComplete.bufferDocumentObverse.clear();
				std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
					std::back_inserter(personSpecificationEnrollComplete.bufferDocumentObverse));				

				personSpecificationEnrollComplete.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
					&personSpecificationEnrollComplete.bufferDocumentObverse[0],
					(int)personSpecificationEnrollComplete.bufferDocumentObverse.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
					
			}
			

			break;
		}
		case 2:
		{			
			if (isEnrollRAV) {
				
				personSpecificationEnroll.bufferDocumentReverse.clear();
				std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
					std::back_inserter(personSpecificationEnroll.bufferDocumentReverse));
				
				personSpecificationEnroll.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
					&personSpecificationEnroll.bufferDocumentReverse[0],
					(int)personSpecificationEnroll.bufferDocumentReverse.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
					
			}
			else
			{
				
				personSpecificationEnrollComplete.bufferDocumentReverse.clear();
				std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
					std::back_inserter(personSpecificationEnrollComplete.bufferDocumentReverse));
				
				personSpecificationEnrollComplete.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
					&personSpecificationEnrollComplete.bufferDocumentReverse[0],
					(int)personSpecificationEnrollComplete.bufferDocumentReverse.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
					
			}
			
						
			break;
		}
		default:
			break;
		}

		imgTemp.clear();
		delete[] rawImage;
	}
}

//void FrameEnrollment::SetImageToTexture(const std::string& path)
//{
//	//personSpecificationEnroll.txtCapture = ASSInterface::Texture2D::Create(path);
//
//}
//
//void FrameEnrollment::SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel)
//{
//	/*personSpecificationEnroll.txtCapture = ASSInterface::Texture2D::Create(data, width, height, channel);*/
//
//}

void FrameEnrollment::CleanSpecification() {
	PersonSpecification pSpec;
	personSpecificationEnrollComplete = pSpec;
	listDetected.clear();
		
}

void FrameEnrollment::ResetChannel()
{
	if (!isEnrollRAV)
	{
		currentIndexVideo = -1;		
		currentIndexVideoDoc = -1;
		std::string strId = &personSpecificationEnrollComplete.id[0];
		if (!strId.empty())
		{
			int id = atoi(strId.c_str());
			RemoveFace(id);			
		}

		CleanSpecification();
		ClearTask();

	}
	
}

void FrameEnrollment::RemoveFace(int id)
{
	innoTask->Remove(id);
}

void FrameEnrollment::SetTextureDocument()
{
	if (isEnrollRAV)
	{
		if (!personSpecificationEnroll.bufferDocumentObverse.empty() &&
			personSpecificationEnroll.txtDocumentObverse == 0)
		{

			personSpecificationEnroll.txtDocumentObverse = 0;
			personSpecificationEnroll.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEnroll.bufferDocumentObverse[0],
				(int)personSpecificationEnroll.bufferDocumentObverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				
		}

		if (!personSpecificationEnroll.bufferDocumentReverse.empty() &&
			personSpecificationEnroll.txtDocumentReverse == 0)
		{
			
			personSpecificationEnroll.txtDocumentReverse = 0;
			personSpecificationEnroll.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEnroll.bufferDocumentReverse[0],
				(int)personSpecificationEnroll.bufferDocumentReverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				
		}
	}
	else
	{
		if (!personSpecificationEnrollComplete.bufferDocumentObverse.empty() &&
			personSpecificationEnrollComplete.txtDocumentObverse == 0)
		{
			
			personSpecificationEnrollComplete.txtDocumentObverse = 0;
			personSpecificationEnrollComplete.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEnrollComplete.bufferDocumentObverse[0],
				(int)personSpecificationEnrollComplete.bufferDocumentObverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
							
		}

		if (!personSpecificationEnrollComplete.bufferDocumentReverse.empty() &&
			personSpecificationEnrollComplete.txtDocumentReverse == 0)
		{
			
			personSpecificationEnrollComplete.txtDocumentReverse = 0;
			personSpecificationEnrollComplete.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEnrollComplete.bufferDocumentReverse[0],
				(int)personSpecificationEnrollComplete.bufferDocumentReverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
							
		}
	}

}

void FrameEnrollment::ClearTask()
{
	if (innoTask != nullptr) {
		innoTask = nullptr;
	}
	if (innoDetect != nullptr)
	{
		innoDetect = nullptr;
	}
}

void FrameEnrollment::SetDocumentObverse(std::vector<unsigned char> dataDocument,
	int width, int height)
{
	int channels = 0, lenght = 0;
	int widthTemp = width;
	int heightTemp = height;
	unsigned char* rawImage = NULL;	
	std::vector<unsigned char> tempImg;

	if (dataDocument.size() > 0)
	{
		std::copy(&dataDocument[0], &dataDocument[0] + dataDocument.size(), std::back_inserter(tempImg));

		rawImage = transImage->Resize(&tempImg[0], &widthTemp, &heightTemp, &lenght);		
		std::vector<unsigned char> imgDoc;
		imgDoc.resize(200 * MB);
		transImage->CaptureDocument(rawImage, &widthTemp, &heightTemp, imgDoc);		

		if (isEnrollRAV)
		{
			personSpecificationEnroll.bufferDocumentObverse.clear();
			std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(),
				std::back_inserter(personSpecificationEnroll.bufferDocumentObverse));			
		}
		else
		{
			personSpecificationEnrollComplete.bufferDocumentObverse.clear();
			std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(), 
				std::back_inserter(personSpecificationEnrollComplete.bufferDocumentObverse));			

		}

		
		imgDoc.clear();
		tempImg.clear();
		delete[] rawImage;
	}
	
}

void FrameEnrollment::SetDocumentReverse(std::vector<unsigned char> dataDocument, int width, int height)
{
	int channels = 0, lenght = 0;
	int widthTemp = width;
	int heightTemp = height;
	unsigned char* rawImage = NULL;
	std::vector<unsigned char> tempImg;

	if (dataDocument.size() > 0) {
		std::copy(&dataDocument[0], &dataDocument[0] + dataDocument.size(), std::back_inserter(tempImg));

		rawImage = transImage->Resize(&tempImg[0], &widthTemp, &heightTemp, &lenght);
		
		std::vector<unsigned char> imgDoc;
		imgDoc.resize(200 * MB);
		transImage->CaptureDocument(rawImage, &widthTemp, &heightTemp, imgDoc);		

		if (isEnrollRAV) {

			personSpecificationEnroll.bufferDocumentReverse.clear();
			std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(),
				std::back_inserter(personSpecificationEnroll.bufferDocumentReverse));			
		}
		else
		{
			personSpecificationEnrollComplete.bufferDocumentReverse.clear();
			std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(),
				std::back_inserter(personSpecificationEnrollComplete.bufferDocumentReverse));			
		}
		
		imgDoc.clear();
		tempImg.clear();
		delete[] rawImage;
	}
	
}
