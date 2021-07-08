#include "FrameEditRegister.h"
#include "imgui/imgui.h"
#include "ASSInterface/Utils/PlatformUtils.h"
#include <SOIL2.h>

FrameEditRegister::FrameEditRegister(ASSInterface::LanguageType language)
{
	lg = language;
	base64 = ASSInterface::Base64::Create();
	transImage = ASSInterface::TransformImage::Create();
}

FrameEditRegister::~FrameEditRegister()
{
}

void FrameEditRegister::Show(bool* p_open)
{	
	static bool popup_channel_open = false;

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);	

	if (!ImGui::Begin("Edit Registration", p_open))
	{
		ImGui::End();
		return;
	}
		
	ImGui::BeginGroup();
	
	ImGui::Image(reinterpret_cast<void*>(intptr_t(personSpecificationEdit.txtGallery)),
		ImVec2{ 120.0f, 149.0f }); 
	
	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("ID:"); ImGui::SameLine();
	ImGui::Text(personSpecificationEdit.id);
	ImGui::Text("First Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name", personSpecificationEdit.name, IM_ARRAYSIZE(personSpecificationEdit.name));
	
	ImGui::Text("Last Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##lastname", personSpecificationEdit.lastname, IM_ARRAYSIZE(personSpecificationEdit.lastname));
	
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
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
	ImGui::Text("Type Person 1:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combotypeperson1", currentItemPersonType1)) {
		for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::TYPES_PERSON); n++) {
			bool is_selected = (currentItemPersonType1 == ConstantApplication::TYPES_PERSON[n]);
			if (ImGui::Selectable(ConstantApplication::TYPES_PERSON[n], is_selected)) {
				currentItemPersonType1 = ConstantApplication::TYPES_PERSON[n];
				strcpy(personSpecificationEdit.type1, currentItemPersonType1);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginGroup();

	SetTextureDocument();

	if (ImGui::ImageButton(reinterpret_cast<void*>(intptr_t(personSpecificationEdit.txtDocumentObverse)),
		ImVec2{ 200.0f, 115.0f })) {
		personSpecificationEdit.txtDocumentObverse = 0;
		int idx = currentIndexVideo - 1;
		if (currentIndexVideo != -1 && *viewVideo[idx])
		{
			shootRequireObverse.on_next(currentIndexVideo - 1);
		}
		else
		{
			*messageStatus = "Select channel.";
		}

	}
	

	if (ImGui::ImageButton(reinterpret_cast<void*>(intptr_t(personSpecificationEdit.txtDocumentReverse)),
		ImVec2{ 200.0f, 115.0f })) {
		personSpecificationEdit.txtDocumentReverse = 0;
		int idx = currentIndexVideo - 1;
		if (currentIndexVideo != -1 && *viewVideo[idx])
		{
			shootRequireReverse.on_next(currentIndexVideo - 1);
		}
		else
		{
			*messageStatus = "Select channel.";
		}

	}
	
	ImGui::EndGroup();	

	ImGui::Separator();
	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPos(ImVec2((10.0f), 290.0f));
	if (ImGui::Button("Search")) {
		if (currentIndexVideo == -1)
		{
			popup_channel_open = true;
			ImGui::SetNextWindowPos(ImGui::GetMousePos());

		}
		else
		{
			Search();
		}
		
	}

	if (popup_channel_open)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("##Popupchannel", &popup_channel_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (!ImGui::IsWindowFocused())
			popup_channel_open = false;

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
					shootChannel.on_next(i);
					
					//ClearTask();
					CreateTaskBiometric(currentIndexVideo);

					Search();
					
				}
				else
				{
					*messageStatus = "The channel is busy.";
				}

				popup_channel_open = false;
			}

		}

		ImGui::End();
		ImGui::PopStyleVar();

	}

	ImGui::SameLine();	
	if (ImGui::Button("Save")) {
		if(VerifyId()) Save();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove")) {
		if(VerifyId()) ImGui::OpenPopup("Delete?");		
	}
		
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{

		ImGui::Text("All the data of the person will be deleted.\nAre you sure?\n\n");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			Remove();
			ImGui::CloseCurrentPopup();			
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear")) {
		ResetListFind(false);
	}

	ImGui::SameLine();
	if (ImGui::Button("Update Biometrics")) {

		if (VerifyId()) {
			viewTemplates = true;
			SetTemplates();
		} 
		
	}
	ImGui::SameLine();
	ImGui::RadioButton("F", &docFace, 1); ImGui::SameLine();
	ImGui::RadioButton("B", &docFace, 2); 
	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 140.0f), 290.0f));
	if (ImGui::Button("Doc. File")) {
		if (VerifyId()) OpenFileDocument();
	}

	ImGui::SetCursorPos(ImVec2((viewportPanelSize.x - 70.0f), 290.0f));
	if (ImGui::Button("Doc. Video")) {
		if (VerifyId()) {
			int index = currentIndexVideo - 1;
			*viewVideo[index] = true;
		}
	}

	ImGui::Separator();
	ImGui::Spacing();

	if (viewTemplates)
	{
		BuildListTemplates();
		ImGui::Separator();
	}		

	BuildListOfSearch();		

	ImGui::End();
}

void FrameEditRegister::Search()
{
	listPersons.clear();
	std::string document = &personSpecificationEdit.document[0];
	if (!document.empty()) SearchByDocument();
	else {
		std::thread ts(&FrameEditRegister::SearchByField, this);
		ts.detach();
	}
	

}

void FrameEditRegister::Save()
{
	std::string strId = &personSpecificationEdit.id[0];
	m_EntitySpec.id = strId;
	m_EntitySpec.name = &personSpecificationEdit.name[0];
	m_EntitySpec.lastName = &personSpecificationEdit.lastname[0];
	m_EntitySpec.type = &personSpecificationEdit.type[0];
	m_EntitySpec.identification = &personSpecificationEdit.document[0];
	m_EntitySpec.type1 = &personSpecificationEdit.type1[0];	

	if (!personSpecificationEdit.bufferDocumentObverse.empty())
	{
		m_EntitySpec.dataDocObverse = base64->base64_encode(&personSpecificationEdit.bufferDocumentObverse[0], 
			(int)personSpecificationEdit.bufferDocumentObverse.size());

	}

	if (!personSpecificationEdit.bufferDocumentReverse.empty())
	{
		m_EntitySpec.dataDocReverse = base64->base64_encode(&personSpecificationEdit.bufferDocumentReverse[0], 
			(int)personSpecificationEdit.bufferDocumentReverse.size());

	}

	dbMongo->Update(m_EntitySpec);

	ASSInterface::EntitySpecification spec;
	m_EntitySpec = spec;
	*messageStatus = "Save register OK.";
}

void FrameEditRegister::ResetListFind(bool close)
{
	if(close) currentIndexVideo = -1;
	
	listPersons.clear();
	PersonSpecification spec;
	personSpecificationEdit = spec;
	viewTemplates = false;
	listTemplates.clear();	
		
}

void FrameEditRegister::BuildListOfSearch()
{

	static int selected_row = -1;

	struct ColumnHeader
	{
		const char* label;
		float size;
	};

	ColumnHeader headers[] =
	{		
		{ "Id", 30 },
		{ "Photo", 70 },
		{ "Name", 150 },
		{ "Last name", 150 },
		{ "Type", 100 },
		{ "Templates", 30 }
	};

	ImGui::BeginChild("##headers");
	ImGui::Columns(IM_ARRAYSIZE(headers), "TableTextureColumns", true);
	ImGui::Separator();
	
	for (ColumnHeader& header : headers)
	{
		ImGui::Text(header.label);		
		ImGui::NextColumn();
	}
	ImGui::Separator();
	

	int sizeList = 0;
	sizeList = listPersons.size();
	ImGui::Columns(6);
	for (int i = 0; i < sizeList; i++)
	{
		std::string itemid = "##" + std::to_string(i);
		if (ImGui::Selectable(itemid.c_str(), i == selected_row, ImGuiSelectableFlags_SpanAllColumns)) {
			selected_row = i;
			
			viewTemplates = false;
			listTemplates.clear();
			strcpy(personSpecificationEdit.id, listPersons[selected_row].id);
			strcpy(personSpecificationEdit.name, listPersons[selected_row].name);
			strcpy(personSpecificationEdit.lastname, listPersons[selected_row].lastname);
			strcpy(personSpecificationEdit.document, listPersons[selected_row].document);
			strcpy(personSpecificationEdit.type, listPersons[selected_row].type);
			strcpy(personSpecificationEdit.type1, listPersons[selected_row].type1);
			const char* typePerson = reinterpret_cast<const char*>(&personSpecificationEdit.type[0]);
			const char* typePerson1 = reinterpret_cast<const char*>(&personSpecificationEdit.type1[0]);
			currentItemPersonType = typePerson;			
			currentItemPersonType1 = typePerson1;
			personSpecificationEdit.txtGallery = listPersons[selected_row].txtGallery;		
			personSpecificationEdit.txtDocumentObverse = listPersons[selected_row].txtDocumentObverse;
			personSpecificationEdit.txtDocumentReverse = listPersons[selected_row].txtDocumentReverse;				
			
		}

		
		ImGui::BeginGroup();		

		ImGui::Text(listPersons[i].id); 
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		if (listPersons[i].txtGallery == 0)
		{
			listPersons[i].txtGallery = SOIL_load_OGL_texture_from_memory(
				&listPersons[i].bufferGallery[0],
				(int)listPersons[i].bufferGallery.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		}		

		if (listPersons[i].txtDocumentObverse == 0 && !listPersons[i].bufferDocumentObverse.empty())
		{
			listPersons[i].txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
				&listPersons[i].bufferDocumentObverse[0],
				(int)listPersons[i].bufferDocumentObverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		}

		if (listPersons[i].txtDocumentReverse == 0 && !listPersons[i].bufferDocumentReverse.empty())
		{
			listPersons[i].txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
				&listPersons[i].bufferDocumentReverse[0],
				(int)listPersons[i].bufferDocumentReverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		}


		ImGui::Image((void*)listPersons[i].txtGallery,
			ImVec2{ 50.0f, 62.0f });

		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::Text(listPersons[i].name);
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::Text(listPersons[i].lastname);
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::Text(listPersons[i].type);
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();		

		if (listPersons[i].templates == 0)
		{
			std::string strId = &listPersons[i].id[0];
			int id = atoi(strId.c_str());
			if (innoTask != NULL)
			{
				listPersons[i].templates = innoTask->GetFaceCount(id);
			}
		}

		ImGui::Text(std::to_string(listPersons[i].templates).c_str());
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::EndGroup();
		
	}

	ImGui::Columns(1);
	ImGui::EndChild();
}

void FrameEditRegister::BuildListTemplates()
{
	static bool removeOrChange = false;
	struct ColumnHeader
	{
		const char* label;
		float size;
	};

	ColumnHeader headers[] =
	{
		{ "Index", 30 },
		{ "Image", 70 },
		{ "Quality", 50 },
		{ "Score", 50 },
		{ "Delete", 70 },
		{ "Change", 70 }
	};

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();		
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(156, 156, 156, 100));
	ImGui::BeginChild("##headtemplate", ImVec2{ viewportPanelSize.x, 150.0f });
	ImGui::Columns(IM_ARRAYSIZE(headers), "TableTextureColumns", true);
	ImGui::Separator();

	for (ColumnHeader& header : headers)
	{
		ImGui::Text(header.label);
		ImGui::NextColumn();
	}
	ImGui::Separator();


	int sizeList = 0;
	sizeList = listTemplates.size();
	ImGui::Columns(6);
	for (int i = 0; i < sizeList; i++)
	{
		
		ImGui::BeginGroup();

		std::string index = std::to_string(listTemplates[i].index);
		std::string nameCtrlDel = "Delete-" + index;
		std::string nameCtrlChange = "Change-" + index;
		ImGui::Text(index.c_str());
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		if (listTemplates[i].txtImage == 0 && !listTemplates[i].bufferImage.empty())
		{
			listTemplates[i].txtImage = SOIL_load_OGL_texture_from_memory(
				&listTemplates[i].bufferImage[0],
				(int)listTemplates[i].bufferImage.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		}

		ImGui::Image((void*)listTemplates[i].txtImage,
			ImVec2{ 50.0f, 62.0f });

		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::Text(std::to_string(listTemplates[i].quality).c_str());
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::Text(std::to_string(listTemplates[i].score).c_str());
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		if (ImGui::Button(nameCtrlDel.c_str())) {
			int index = GetIndexTemplate(nameCtrlDel);
			RemoveFace(index);
			UpdateIndexDatabase();		
			removeOrChange = true;
		}		
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		if (ImGui::Button(nameCtrlChange.c_str())) {
			int index = GetIndexTemplate(nameCtrlChange);
			std::string strId = &personSpecificationEdit.id[0];
			int id = atoi(strId.c_str());		
			int idVideo = currentIndexVideo - 1;
			if (*viewVideo[idVideo])
			{				
				innoTask->SetId(id);
				innoTask->SetIndex(index);

			}
			else
			{
				OpenFile(id, SET_TEMPLATE_OF_FILES, index);
			}
			
			
		}
		ImGui::SetItemAllowOverlap();
		ImGui::NextColumn();

		ImGui::EndGroup();

	}

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Spacing();

	FootViewTemplates();
	ImGui::PopStyleColor();
	ImGui::EndChild();

	if (removeOrChange)
	{
		SetTemplates();
		removeOrChange = false;
	}

}

void FrameEditRegister::SearchByDocument()
{
	std::string document = &personSpecificationEdit.document[0];
	ResetListFind(false);

	if (!document.empty())
	{
		dbMongo->GetByDocument(document.c_str());

		if (!dbMongo->GetEntitySpecification().id.empty())
		{
			int id = atoi(dbMongo->GetEntitySpecification().id.c_str());
			personSpecificationEdit.templates = innoTask->GetFaceCount(id);

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
			if (!imgData.empty())
			{
				std::string decImg = base64->base64_decode(imgData);
				std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
				const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);

				personSpecificationEdit.txtGallery = 0;
				personSpecificationEdit.txtGallery = SOIL_load_OGL_texture_from_memory(
					templateData, (int)cstr.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				templateData = NULL;
			}

			imgData = dbMongo->GetEntitySpecification().dataDocObverse;
			if (!imgData.empty())
			{
				std::string decImg = base64->base64_decode(imgData);
				std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
				const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);

				personSpecificationEdit.txtDocumentObverse= 0;
				personSpecificationEdit.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
					templateData, (int)cstr.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				templateData = NULL;
			}

			imgData = dbMongo->GetEntitySpecification().dataDocReverse;
			if (!imgData.empty())
			{
				std::string decImg = base64->base64_decode(imgData);
				std::vector<char> cstr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
				const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&cstr[0]);

				personSpecificationEdit.txtDocumentReverse = 0;
				personSpecificationEdit.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
					templateData, (int)cstr.size(),
					SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
				templateData = NULL;
			}


			strcpy(personSpecificationEdit.type, dbMongo->GetEntitySpecification().type.c_str());
			currentItemPersonType = reinterpret_cast<const char*>(personSpecificationEdit.type);

			if (!dbMongo->GetEntitySpecification().type1.empty())
			{
				strcpy(personSpecificationEdit.type1, dbMongo->GetEntitySpecification().type1.c_str());
				currentItemPersonType1 = reinterpret_cast<const char*>(personSpecificationEdit.type1);

			}

		}

	}
}

void FrameEditRegister::SearchByField()
{
	std::string type = &personSpecificationEdit.type[0];
	std::string name = &personSpecificationEdit.name[0];
	std::string lastName = &personSpecificationEdit.lastname[0];

	if (!type.empty()) dbMongo->GetByType(type);
	else if (!name.empty()) dbMongo->GetByName(name);
	else if (!lastName.empty()) dbMongo->GetByLastName(lastName);
	TransforToPersonSpecification();

}

void FrameEditRegister::TransforToPersonSpecification()
{
	if (dbMongo->ListFind().size() > 0)
	{		
		parallel_for(blocked_range<size_t>(0, dbMongo->ListFind().size()),
			[=](const blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i != r.end(); ++i) {					
					AddPersonToList(dbMongo->ListFind().at(i));
				}
			}
		);

	}
}

void FrameEditRegister::AddPersonToList(ASSInterface::EntitySpecification specEnt)
{
	PersonSpecification spec;
	std::string val = specEnt.id;
	std::string name = specEnt.name;
	std::string lastName = specEnt.lastName;
	std::string type = specEnt.type;
	std::string type1 = specEnt.type1;
	std::string document = specEnt.identification;
	std::string gallery = specEnt.dataImage;
	std::string imgDocObserver = specEnt.dataDocObverse;
	std::string imgDocReverse = specEnt.dataDocReverse;

	std::string decImg = base64->base64_decode(gallery);
	std::vector<char> imgStr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
	unsigned char* imgDB = reinterpret_cast<unsigned char*>(&imgStr[0]);

	if (!imgDocObserver.empty())
	{
		std::string docImg = base64->base64_decode(imgDocObserver);
		std::vector<char> imgDocStr(docImg.c_str(), docImg.c_str() + docImg.size() + 1);
		unsigned char* imgDoc = reinterpret_cast<unsigned char*>(&imgDocStr[0]);
		spec.bufferDocumentObverse.assign(imgDoc, imgDoc + imgDocStr.size());
	}

	if (!imgDocReverse.empty())
	{
		std::string docImg = base64->base64_decode(imgDocReverse);
		std::vector<char> imgDocStr(docImg.c_str(), docImg.c_str() + docImg.size() + 1);
		unsigned char* imgDoc = reinterpret_cast<unsigned char*>(&imgDocStr[0]);
		spec.bufferDocumentReverse.assign(imgDoc, imgDoc + imgDocStr.size());
	}

	strcpy(spec.id, val.c_str());
	strcpy(spec.lastname, lastName.c_str());
	strcpy(spec.name, name.c_str());
	strcpy(spec.document, document.c_str());
	strcpy(spec.type, type.c_str());
	strcpy(spec.type1, type1.c_str());

	spec.bufferGallery.assign(imgDB, imgDB + imgStr.size());		

	listPersons.push_back(spec);
}

void FrameEditRegister::Remove()
{	
	std::string strId = &personSpecificationEdit.id[0];

	int id = atoi(strId.c_str());

	innoTask->Remove(id);
	dbMongo->Delete(strId);
	dbMongo->DeleteImages(strId);
	ResetListFind(false);
	
}

bool FrameEditRegister::VerifyId()
{
	std::string strId = &personSpecificationEdit.id[0];

	return !strId.empty() && innoTask != nullptr;
	
}

void FrameEditRegister::SetTemplates()
{
	int idxVideo = currentIndexVideo - 1;

	if (*viewVideo[idxVideo])
	{
		*viewVideo[idxVideo] = false;
	}
	
	innoTask->SetId(-1);
	innoTask->SetIndex(-1);

	listTemplates.clear();

	std::string strId = &personSpecificationEdit.id[0];

	dbMongo->GetImages(strId);

	std::vector<ASSInterface::EntityImage> images = dbMongo->ListImages();

	if (!images.empty())
	{
		for (size_t i = 0; i < images.size(); i++)
		{
			ASSInterface::TemplateSpecification specTemp;

			int id = atoi(images[i].id.c_str());
			std::vector<int> dataBiometric = innoTask->GetFaceQuality(id, images[i].index);

			specTemp.index = images[i].index;			
			specTemp.quality = dataBiometric[0];
			specTemp.score = dataBiometric[1];

			std::string strDecImg = base64->base64_decode(images[i].data);
			std::vector<char> imgChr(strDecImg.c_str(), strDecImg.c_str() + strDecImg.size() + 1);
			unsigned char* img = reinterpret_cast<unsigned char*>(&imgChr[0]);			
			specTemp.bufferImage.assign(img, img + imgChr.size());

			listTemplates.push_back(specTemp);
		}
	}

	images.clear();
	
}

void FrameEditRegister::FootViewTemplates()
{
	
	std::string strId = &personSpecificationEdit.id[0];
	int id = atoi(strId.c_str());
	
	ImGui::BeginGroup();
	ImGui::Text("Add templates:"); ImGui::SameLine();
	if (ImGui::Button("File")) {
		OpenFile(id, ADD_TEMPLATES_OF_FILES);
	}
	ImGui::SameLine();
	if (ImGui::Button("Video")) {
		int index = currentIndexVideo - 1;
		*viewVideo[index] = true;
		shootChannel.on_next(index);
		std::string strId = &personSpecificationEdit.id[0];
		int id = atoi(strId.c_str());
		innoTask->SetId(id);
	}

	ImGui::EndGroup();
}

void FrameEditRegister::OpenFile(int id, int task, int index)
{

	std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile(
		"Images (*.png)\0*.png\0(*.jpg)\0*.jpg\0");
	if (!filePaths.empty())
	{		
		innoDetect->BuildTemplatesFromPersonFiles(filePaths, task, id, index);
		isFileButton = false;
	}
}

void FrameEditRegister::ClearTask()
{
	if (innoTask != nullptr) {
		innoTask = nullptr;
	}
	if (innoDetect != nullptr)
	{
		innoDetect = nullptr;
	}
}

void FrameEditRegister::CreateTaskBiometric(int channel)
{
	//innoTask = ASSInterface::ExecuteTask::CreateInnoTask(channel);
	innoDetect = ASSInterface::Detection::CreateInnoDetected(channel);
	//innoTask->SetDatabase(dbMongo);
	
	ObserverDetected();
	//ObserverTask();
}

void FrameEditRegister::ObserverDetected()
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

void FrameEditRegister::ObserverTask()
{
	auto endTaskObservable = innoTask->EndTask()
		.map([](int result) {
		return result;
			});

	auto subscriptionEndTask = endTaskObservable
		.subscribe([this](int result) {
			if (result != 0)
			{
				SetTemplates();
			}
		});

	subscriptionEndTask.clear();


}

std::vector<std::string> FrameEditRegister::Split(const std::string& s, char delimiter)
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

int FrameEditRegister::GetIndexTemplate(std::string nameControl)
{
	std::vector<std::string> tokens = Split(nameControl, '-');
	if (tokens.size() > 1)
	{
		return atoi(tokens[1].c_str());
	}

	return -1;
}

void FrameEditRegister::RemoveFace(int index)
{
	std::string strId = &personSpecificationEdit.id[0];
	int id = atoi(strId.c_str());

	innoTask->RemoveFace(id, index);
	ASSInterface::EntityImage ent;
	ent.id = strId;
	ent.index = index;
	dbMongo->DeleteImage(ent);
		
}

void FrameEditRegister::UpdateIndexDatabase()
{
	std::string id = &personSpecificationEdit.id[0];

	dbMongo->GetImages(id);

	std::vector<ASSInterface::EntityImage> images = dbMongo->ListImages();

	if (!images.empty())
	{
		for (size_t i = 0; i < images.size(); i++)
		{

			if (images[i].index != i)
			{
				ASSInterface::EntityImage ent;
				ent.id = id;
				ent.index = images[i].index;
				dbMongo->Update(ent, i);
			}
			
		}
	}

	images.clear();
}

void FrameEditRegister::SetDocumentObverse(std::vector<unsigned char> dataDocument,
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

		personSpecificationEdit.bufferDocumentObverse.clear();
		std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(),
			std::back_inserter(personSpecificationEdit.bufferDocumentObverse));


		imgDoc.clear();
		tempImg.clear();
		delete[] rawImage;
	}

}

void FrameEditRegister::SetDocumentReverse(std::vector<unsigned char> dataDocument, int width, int height)
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

		personSpecificationEdit.bufferDocumentReverse.clear();
		std::copy(&imgDoc[0], &imgDoc[0] + imgDoc.size(),
			std::back_inserter(personSpecificationEdit.bufferDocumentReverse));

		imgDoc.clear();
		tempImg.clear();
		delete[] rawImage;
	}

}

void FrameEditRegister::SetTextureDocument() {

	if (!personSpecificationEdit.bufferDocumentObverse.empty() &&
		personSpecificationEdit.txtDocumentObverse == 0)
	{

		personSpecificationEdit.txtDocumentObverse = 0;
		personSpecificationEdit.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
			&personSpecificationEdit.bufferDocumentObverse[0],
			(int)personSpecificationEdit.bufferDocumentObverse.size(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	}

	if (!personSpecificationEdit.bufferDocumentReverse.empty() &&
		personSpecificationEdit.txtDocumentReverse == 0)
	{

		personSpecificationEdit.txtDocumentReverse = 0;
		personSpecificationEdit.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
			&personSpecificationEdit.bufferDocumentReverse[0],
			(int)personSpecificationEdit.bufferDocumentReverse.size(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	}
}

void FrameEditRegister::OpenFileDocument()
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
			personSpecificationEdit.bufferDocumentObverse.clear();
			std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
				std::back_inserter(personSpecificationEdit.bufferDocumentObverse));

			personSpecificationEdit.txtDocumentObverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEdit.bufferDocumentObverse[0],
				(int)personSpecificationEdit.bufferDocumentObverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

			break;
		}
		case 2:
		{
			personSpecificationEdit.bufferDocumentReverse.clear();
			std::copy(&imgTemp[0], &imgTemp[0] + imgTemp.size(),
				std::back_inserter(personSpecificationEdit.bufferDocumentReverse));

			personSpecificationEdit.txtDocumentReverse = SOIL_load_OGL_texture_from_memory(
				&personSpecificationEdit.bufferDocumentReverse[0],
				(int)personSpecificationEdit.bufferDocumentReverse.size(),
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


			break;
		}
		default:
			break;
		}

		imgTemp.clear();
		delete[] rawImage;
	}
}
