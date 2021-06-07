#include "FrameControlEntry.h"
#include "imgui/imgui.h"
#include <SOIL2.h>

FrameControlEntry::FrameControlEntry(ASSInterface::LanguageType language)
{
	lg = language;
	txtButtom = ASSInterface::Texture2D::Create("assets/textures/cctv.png");
}

FrameControlEntry::~FrameControlEntry()
{
}

void FrameControlEntry::Show(bool* p_open, PersonSpecification& personSpec)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Entry Control", p_open))
	{
		ImGui::End();
		return;
	}

	ShowScreenHead();

	if (indexRemove != -1) {		
		RemovePersonDetected(indexRemove);
		indexRemove = -1;
	}

	SetNewsDetected();

	for (int i = 0; i < listShowDetected.size(); i++)
	{
		ShowDetected(i, personSpec);
	}
	
	ImGui::End();
}

void FrameControlEntry::ShowScreenHead()
{
	static bool popup_open = false;	

	ImTextureID id = (ImTextureID)txtButtom->GetRendererID();

	ImGui::SetCursorPos(ImVec2(10.0f, 30.0f));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_REGISTERED, lg).c_str()); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(90.0f, 30.0f));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_CAPTURED, lg).c_str()); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(220.0f, 30.0f));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_DATA, lg).c_str()); ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2(300.0f, 25.0f));
	if (ImGui::ImageButton(id, ImVec2(24, 24))) {
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
					*viewVideo[i] = true;					
					shootChannel.on_next(i);
				}
				popup_open = false;
			}
			
		}

		ImGui::End();
		ImGui::PopStyleVar();
		
	}

	ImGui::Separator();
	ImGui::Spacing();

}

void FrameControlEntry::ShowDetected(int index, PersonSpecification& personSpec)
{	
	static const char* current_place = NULL;
	std::string nameScreen = "Detect-" + std::to_string(index);
	std::string firtsName = &listShowDetected[index].name[0];
	std::string lastName = &listShowDetected[index].lastname[0];
	std::string name = firtsName + " " + lastName;

	ImGui::BeginChild(nameScreen.c_str(), ImVec2(0, 150), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);

	ImGui::Image((void*)listShowDetected[index].txtGallery, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::Image((void*)listShowDetected[index].txtCapture, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_NAME, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::Text(name.c_str());
	
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listShowDetected[index].document);
	
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_CHANNEL, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::Text(listShowDetected[index].channel);

	ImGui::Text(ASSInterface::Label::GetLabel(LBL_MATCH, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::Text(listShowDetected[index].match);

	ImGui::Text(ASSInterface::Label::GetLabel(LBL_DATE, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listShowDetected[index].date);

	/*ImGui::Text("Gallery:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::InputText("##gallery", listShowDetected[index].type, IM_ARRAYSIZE(listShowDetected[index].type),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));*/
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_GALLERY, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::Text(listShowDetected[index].type);

	ImGui::EndGroup();

	ImGui::BeginGroup();

	ImGui::Text(std::to_string(index + 1).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	if (ImGui::BeginCombo("##combo", listShowDetected[index].place)) {
		for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::DESTINATION_PLACES); n++) {
			bool is_selected = (current_place == ConstantApplication::DESTINATION_PLACES[n]);
			if (ImGui::Selectable(ConstantApplication::DESTINATION_PLACES[n], is_selected)) {
				current_place = ConstantApplication::DESTINATION_PLACES[n];
				strcpy(listShowDetected[index].place, current_place);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Button("Authorize", ImVec2(100.0f, 20.0f))) {
		indexRemove = GetIndexDetected(nameScreen);
		SetPersonInside(indexRemove, personSpec);
	}
	ImGui::PopStyleVar(1);
	ImGui::EndGroup();

	ImGui::PopFont();

	ImGui::EndChild();
}

void FrameControlEntry::SetPersonInside(int index, PersonSpecification& personSpec) {

	std::string currentPlace = &listShowDetected[index].place[0];

	if (!currentPlace.empty())
	{
		std::string dateDetect = ASSInterface::DateTime::Now();
		strcpy(personSpec.id, listShowDetected[index].id);
		strcpy(personSpec.name, listShowDetected[index].name);
		strcpy(personSpec.document, listShowDetected[index].document);
		strcpy(personSpec.date, dateDetect.c_str());
		strcpy(personSpec.type, listShowDetected[index].type);
		strcpy(personSpec.place, listShowDetected[index].place);
		strcpy(personSpec.match, listShowDetected[index].match);
		strcpy(personSpec.channel, listShowDetected[index].channel);

		personSpec.txtCapture = listShowDetected[index].txtCapture;
		personSpec.txtGallery = listShowDetected[index].txtGallery;

		ASSInterface::EntityEvent entEvent;
		entEvent.id = &personSpec.id[0];
		entEvent.date = ASSInterface::DateTime::NowToLong();
		entEvent.type = ASSInterface::EventAppType::inside;
		entEvent.description = &personSpec.place[0];

		dbMongo->Add(entEvent);
	}
}

void FrameControlEntry::SetNewsDetected()
{
	bool flagExist = true;
	std::lock_guard lock(mutexList);
	for (int i = 0; i < listDetectedTemp.size(); i++)
	{
		for (int j = 0; j < listDetected.size(); j++)
		{
			std::string idDetect = &listDetected[j].id[0];
			std::string idTemp = &listDetectedTemp[i].id[0];
			int idIntDet = atoi(idDetect.c_str());
			int idIntTemp = atoi(idTemp.c_str());
			if (idIntDet == idIntTemp)
			{
				flagExist = false;
			}
		}

		if (flagExist)
		{
			listDetectedTemp[i].txtCapture = SOIL_load_OGL_texture_from_memory(
				&listDetectedTemp[i].bufferCapture[0],
				(int)listDetectedTemp[i].bufferCapture.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
			listDetectedTemp[i].txtGallery = SOIL_load_OGL_texture_from_memory(
				&listDetectedTemp[i].bufferGallery[0],
				(int)listDetectedTemp[i].bufferGallery.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

			if (listDetectedTemp[i].txtCapture != 0 && listDetectedTemp[i].txtGallery != 0)
			{
				listDetected.push_back(listDetectedTemp.at(i));
				listShowDetected.push_back(listDetectedTemp.at(i));
			}

		}

	}

	listDetectedTemp.clear();
}

int FrameControlEntry::GetIndexDetected(std::string nameControl) {
	std::vector<std::string> tokens = Split(nameControl, '-');
	if (tokens.size() > 1)
	{
		return atoi(tokens[1].c_str());
	}

	return -1;
}

std::vector<std::string> FrameControlEntry::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}