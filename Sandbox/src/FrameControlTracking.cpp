#include "hzpch.h"
#include "FrameControlTracking.h"
#include "imgui/imgui.h"
#include <SOIL2.h>

FrameControlTracking::FrameControlTracking()
{
	txtButtom = ASSInterface::Texture2D::Create("assets/textures/cctv.png");
}

FrameControlTracking::~FrameControlTracking()
{
}

void FrameControlTracking::Show(bool* p_open, int index)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	std::string nameWindow = "Tracking Control " + std::to_string(index);
	if (!ImGui::Begin(nameWindow.c_str(), p_open))
	{
		ImGui::End();
		return;
	}

	ShowScreenHead(index);
	SetNewsDetected();
	for (int i = 0; i < listDetected.size(); i++)
	{
		ShowDetected(i);
	}

	ImGui::End();

}

void FrameControlTracking::ShowScreenHead(int index)
{	
	ImTextureID id = (ImTextureID)txtButtom->GetRendererID();

	ImGui::SetCursorPos(ImVec2(10.0f, 30.0f));
	ImGui::Text("Registered"); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(90.0f, 30.0f));
	ImGui::Text("Captured"); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(220.0f, 30.0f));
	ImGui::Text("Data"); ImGui::SameLine();

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

		for (size_t i = 0; i < IM_ARRAYSIZE(ConstantApplication::QUANTITY_CHANNELS); i++) {

			bool is_selected = (current_item == ConstantApplication::QUANTITY_CHANNELS[i]);

			if (ImGui::Selectable(ConstantApplication::QUANTITY_CHANNELS[i], is_selected))
			{
				current_item = ConstantApplication::QUANTITY_CHANNELS[i];
				ImGui::SetItemDefaultFocus();
				if (!*viewVideo[i])
				{
					*viewVideo[i] = true;
					auto tupleChannel = std::make_tuple(i, index);
					shootChannel.on_next(tupleChannel);
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

void FrameControlTracking::ShowDetected(int index)
{
	std::string nameScreen = "Detect-" + std::to_string(index);
	std::string firtsName = &listDetected[index].name[0];
	std::string lastName = &listDetected[index].lastname[0];
	std::string name = firtsName + " " + lastName;

	ImGui::BeginChild(nameScreen.c_str(), ImVec2(0, 130), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);

	ImGui::Image((void*)listDetected[index].txtGallery, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::Image((void*)listDetected[index].txtCapture, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::Text(name.c_str());

	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listDetected[index].document);

	ImGui::Text("Channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::Text(listDetected[index].channel);

	ImGui::Text("Match:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::Text(listDetected[index].match);

	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listDetected[index].date);

	ImGui::Text("Gallery:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::Text(listDetected[index].type);

	ImGui::EndGroup();	

	ImGui::PopFont();

	ImGui::EndChild();
}

void FrameControlTracking::SetNewsDetected()
{	
	std::lock_guard lock(mutexList);
	for (int i = 0; i < listDetectedTemp.size(); i++)
	{
		bool flagExist = true;

		std::string strIdTemp = &listDetectedTemp[i].id[0];		
		int idTemp = atoi(strIdTemp.c_str());

		if (idTemp != 0)
		{
			for (int j = 0; j < listDetected.size(); j++)
			{
				std::string strIdDetect = &listDetected[j].id[0];			
				int idDetected = atoi(strIdDetect.c_str());				

				if (idDetected == idTemp)
				{
					flagExist = false;
				}
			}

		}

		if (flagExist)
		{
			listDetectedTemp[i].txtCapture = SOIL_load_OGL_texture_from_memory(
				&listDetectedTemp[i].bufferCapture[0],
				(int)listDetectedTemp[i].bufferCapture.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

			if (listDetectedTemp[i].bufferGallery.size() > 0)
			{
				listDetectedTemp[i].txtGallery = SOIL_load_OGL_texture_from_memory(
					&listDetectedTemp[i].bufferGallery[0],
					(int)listDetectedTemp[i].bufferGallery.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
			}
			
			listDetectedTemp[i].bufferCapture.clear();
			listDetectedTemp[i].bufferGallery.clear();
			listDetectedTemp[i].bufferDocumentObverse.clear();
			listDetectedTemp[i].bufferDocumentReverse.clear();
			listDetectedTemp[i].faceSerialized.clear();

			if (listDetectedTemp[i].txtCapture != 0 && listDetectedTemp[i].txtGallery != 0)
			{
				listDetected.push_back(listDetectedTemp.at(i));			
				SaveEventTrack(listDetectedTemp.at(i));
			}
			else if (listDetectedTemp[i].txtCapture != 0) {
				listDetected.push_back(listDetectedTemp.at(i));
				SaveEventTrackUnidentified(listDetectedTemp.at(i));
			}
		}
	}

	listDetectedTemp.clear();
}

void FrameControlTracking::SaveEventTrack(PersonSpecification& personSpec)
{
	std::string channel = &personSpec.channel[0];
	ASSInterface::EntityEvent entEvent;
	entEvent.id = &personSpec.id[0];
	entEvent.date = ASSInterface::DateTime::NowToLong();
	entEvent.type = ASSInterface::EventAppType::detectedInside;
	entEvent.description = "Person detected on channel: " + channel;

	dbMongo->Add(entEvent);
}

void FrameControlTracking::SaveEventTrackUnidentified(PersonSpecification& personSpec)
{
	std::string channel = &personSpec.channel[0];
	ASSInterface::EntityEvent entEvent;
	entEvent.id = -1;
	entEvent.date = ASSInterface::DateTime::NowToLong();
	entEvent.type = ASSInterface::EventAppType::unidentified;
	entEvent.description = "Person unidentified detected on channel: " + channel;

	dbMongo->Add(entEvent);
}
