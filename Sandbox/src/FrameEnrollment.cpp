#include "hzpch.h"
#include "FrameEnrollment.h"
#include "ASSInterface/Utils/PlatformUtils.h"
#include "imgui/imgui.h"

FrameEnrollment::FrameEnrollment()
{
	base64 = ASSInterface::Base64::Create();
	SetImageToTexture("assets/textures/avatar.png");
}

FrameEnrollment::~FrameEnrollment()
{
}

void FrameEnrollment::Show(bool* p_open, PersonSpecification& personSpec)
{
	static const char* currentItemPersonType = NULL;
	static const char* current_video = NULL;
	std::string dateEnroll = ASSInterface::DateTime::Now();
	strcpy(personSpecificationEnroll.date, dateEnroll.c_str());

	if (currentIndexVideo == 0 && !*viewVideo[0]) currentIndexVideo = -1;
	if (currentIndexVideo == 1 && !*viewVideo[1]) currentIndexVideo = -1;

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment", p_open)) 
	{
		ImGui::End();
		return;
	}
	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 150.0f) * 0.5f, 30.0f));
	ImGui::Image((void*)personSpecificationEnroll.txtCapture->GetRendererID(), ImVec2{ 150.0f, 187.0f });

	if (buttonsCaptureEnroll)
	{
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("File")) {
			OpenFile();

		}

		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		if (ImGui::BeginCombo("##combovideo", current_video))
		{
			for (int n = 0; n < IM_ARRAYSIZE(itemsVideo); n++)
			{
				bool is_selected = (current_video == itemsVideo[n]);
				if (ImGui::Selectable(itemsVideo[n], is_selected)) {
					current_video = itemsVideo[n];
					ImGui::SetItemDefaultFocus();
					if (!*viewVideo[n])
					{
						*viewVideo[n] = true;
						currentIndexVideo = n;
					}

				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("ID:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##id", personSpecificationEnroll.id, IM_ARRAYSIZE(personSpecificationEnroll.id));
	ImGui::Text("Edit Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(250);
	ImGui::InputText("##name", personSpecificationEnroll.name, IM_ARRAYSIZE(personSpecificationEnroll.name));
	ImGui::Text("Edit Document:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##document", personSpecificationEnroll.document, IM_ARRAYSIZE(personSpecificationEnroll.document));
	ImGui::Text("Type Person:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combotypeperson", currentItemPersonType)) {
		for (int n = 0; n < IM_ARRAYSIZE(itemsTypePerson); n++) {
			bool is_selected = (currentItemPersonType == itemsTypePerson[n]);
			if (ImGui::Selectable(itemsTypePerson[n], is_selected)) {
				currentItemPersonType = itemsTypePerson[n];
				strcpy(personSpecificationEnroll.type, currentItemPersonType);				
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Edit Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##date", personSpecificationEnroll.date, IM_ARRAYSIZE(personSpecificationEnroll.date), 
		ImGuiInputTextFlags_AllowTabInput | (readOnlyDate ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::Spacing();

	if (ImGui::Button("Save Register")) {	
		std::string autorized = "Not autorized";
		strcpy(personSpecificationEnroll.state, autorized.c_str());
		SavePerson(personSpec);
	}	
	ImGui::SameLine();
	ImGui::Text("Place:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combo", personSpecificationEnroll.place)) {
		for (int n = 0; n < IM_ARRAYSIZE(itemsTarget); n++) {
			bool is_selected = (personSpecificationEnroll.place == itemsTarget[n]);
			if (ImGui::Selectable(itemsTarget[n], is_selected)) {
				strcpy(personSpecificationEnroll.place, itemsTarget[n]);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	if (ImGui::Button("Authorize entry")) {
		std::string autorized = "Autorized";
		strcpy(personSpecificationEnroll.state, autorized.c_str());
		SavePerson(personSpec);		
	}

	//if (enrolledPerson)
	//{
	//	//ShowScreenDetect();
	//}

	ImGui::End();
}

void FrameEnrollment::SavePerson(PersonSpecification& personSpec) {

	ASSInterface::EntitySpecification entSpec;
	entSpec.id = &personSpecificationEnroll.id[0]; 
	entSpec.name = &personSpecificationEnroll.name[0];
	entSpec.identification = &personSpecificationEnroll.document[0];
	entSpec.date = ASSInterface::DateTime::NowToLong();
	entSpec.type = &personSpecificationEnroll.type[0];

	if (!entSpec.id.empty() && !entSpec.name.empty() &&
		!entSpec.identification.empty() && !entSpec.type.empty() && 
		!personSpecificationEnroll.bufferCapture.empty())
	{				
		entSpec.data = base64->base64_encode(&personSpecificationEnroll.bufferCapture[0], personSpecificationEnroll.bufferCapture.size());
		dbMongo->Add(entSpec);

		strcpy(personSpec.id, personSpecificationEnroll.id);
		strcpy(personSpec.name, personSpecificationEnroll.name);
		strcpy(personSpec.document, personSpecificationEnroll.document);
		strcpy(personSpec.date, personSpecificationEnroll.date);
		strcpy(personSpec.type, personSpecificationEnroll.type);
		strcpy(personSpec.place, personSpecificationEnroll.place);
		strcpy(personSpec.state, personSpecificationEnroll.state);
		personSpec.txtCapture = personSpecificationEnroll.txtCapture;
		personSpec.txtGallery = personSpecificationEnroll.txtCapture;
		personSpec.bufferCapture.clear();
		personSpec.bufferCapture.assign(&personSpecificationEnroll.bufferCapture[0],
			&personSpecificationEnroll.bufferCapture[0] + personSpecificationEnroll.bufferCapture.size());
	
	}
	
}

void FrameEnrollment::SetBufferCapture(std::vector<unsigned char> buff) {
	personSpecificationEnroll.bufferCapture.clear();
	personSpecificationEnroll.bufferCapture.assign(&buff[0], &buff[0] + buff.size());
}

void FrameEnrollment::OpenFile()
{
	std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile("Images (*.png)\0*.png\0");
	if (!filePaths.empty())
	{
		for (int i = 0; i < filePaths.size(); i++)
		{
			if (i == 0)
			{
				SetImageToTexture(filePaths[i]);
			}
			ASS_INFO("Files: {0}", filePaths[i]);
		}

	}
}

void FrameEnrollment::SetImageToTexture(const std::string& path)
{
	personSpecificationEnroll.txtCapture = ASSInterface::Texture2D::Create(path);

}

void FrameEnrollment::SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel)
{
	personSpecificationEnroll.txtCapture = ASSInterface::Texture2D::Create(data, width, height, channel);

}

void FrameEnrollment::CleanSpecification() {
	PersonSpecification pSpec;
	personSpecificationEnroll = pSpec;

}
