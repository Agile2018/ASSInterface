#include "FrameEditRegister.h"
#include "imgui/imgui.h"
#include <SOIL2.h>

FrameEditRegister::FrameEditRegister(ASSInterface::LanguageType language)
{
	lg = language;
	base64 = ASSInterface::Base64::Create();
}

FrameEditRegister::~FrameEditRegister()
{
}

void FrameEditRegister::Show(bool* p_open)
{
	
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment", p_open))
	{
		ImGui::End();
		return;
	}

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 250.0f) * 0.25f, 30.0f));
	//SetTextureNewDetected();
	ImGui::Image((void*)personSpecificationEdit.txtGallery,
		ImVec2{ 120.0f, 149.0f });

	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("First Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##name", personSpecificationEdit.name, IM_ARRAYSIZE(personSpecificationEdit.name));
	
	ImGui::Text("Last Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##lastname", personSpecificationEdit.lastname, IM_ARRAYSIZE(personSpecificationEdit.lastname));
	
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##document", personSpecificationEdit.document, IM_ARRAYSIZE(personSpecificationEdit.document), ImGuiInputTextFlags_CharsNoBlank);

	ImGui::Text("Type Person:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combotypeperson", currentItemPersonType)) {
		for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::TYPES_PERSON); n++) {
			bool is_selected = (currentItemPersonType == ConstantApplication::TYPES_PERSON[n]);
			if (ImGui::Selectable(ConstantApplication::TYPES_PERSON[n], is_selected)) {
				currentItemPersonType = ConstantApplication::TYPES_PERSON[n];
				strcpy(personSpecificationEdit.type, currentItemPersonType);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::EndGroup();

	ImGui::Separator();
	ImGui::Spacing();
	
	if (ImGui::Button("Search")) {
		Search();
	}
	ImGui::SameLine();	
	if (ImGui::Button("Save")) {
		Save();
	}

	ImGui::End();
}

void FrameEditRegister::Search()
{
	std::string document = &personSpecificationEdit.document[0];
	if (!document.empty())
	{
		dbMongo->GetByDocument(document.c_str());

		if (!dbMongo->GetEntitySpecification().id.empty())
		{
			m_EntitySpec = dbMongo->GetEntitySpecification();
			long dte = dbMongo->GetEntitySpecification().date;
			std::string strDate = ASSInterface::DateTime::LongToString(dte);
			strcpy(personSpecificationEdit.id, dbMongo->GetEntitySpecification().id.c_str());
			strcpy(personSpecificationEdit.name, dbMongo->GetEntitySpecification().name.c_str());
			strcpy(personSpecificationEdit.lastname, dbMongo->GetEntitySpecification()
				.lastName.c_str());
			strcpy(personSpecificationEdit.document, dbMongo->GetEntitySpecification().identification.c_str());
			strcpy(personSpecificationEdit.date, strDate.c_str());

			std::string imgData = dbMongo->GetEntitySpecification().dataImage;
			std::string decImg = base64->base64_decode(imgData);
			std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
			const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);
			
			personSpecificationEdit.txtGallery = 0;
			personSpecificationEdit.txtGallery = SOIL_load_OGL_texture_from_memory(
				templateData, (int)cstr.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);		
			strcpy(personSpecificationEdit.type, dbMongo->GetEntitySpecification().type.c_str());
			currentItemPersonType = reinterpret_cast<const char*>(personSpecificationEdit.type);
			
		}

	}
		 
}

void FrameEditRegister::Save()
{
	m_EntitySpec.name = &personSpecificationEdit.name[0];
	m_EntitySpec.lastName = &personSpecificationEdit.lastname[0];
	m_EntitySpec.type = &personSpecificationEdit.type[0];
	m_EntitySpec.identification = &personSpecificationEdit.document[0];
	dbMongo->Update(m_EntitySpec);

	ASSInterface::EntitySpecification spec;
	m_EntitySpec = spec;
	*messageStatus = "Save register OK.";
}
