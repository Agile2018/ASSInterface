#include "hzpch.h"
#include "FrameControlEntry.h"
#include "imgui/imgui.h"

FrameControlEntry::FrameControlEntry()
{
}

FrameControlEntry::~FrameControlEntry()
{
}

void FrameControlEntry::Show(bool* p_open, PersonSpecification& personSpec)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Control Entry", p_open))
	{
		ImGui::End();
		return;
	}

	ShowScreenHead();

	//ShowLastAction();

	//ShowUnidentified(menuEnroll, indexUnidentified);

	if (indexRemove != -1) {		
		RemovePersonDetected(indexRemove);
		indexRemove = -1;
	}

	for (int i = 0; i < listDetected.size(); i++)
	{
		ShowDetected(i, personSpec);
	}
	
	ImGui::End();
}

void FrameControlEntry::ShowScreenHead()
{
	std::string dateDetect = ASSInterface::DateTime::Now();
	const char* items[] = { "Video 1", "Video 2" };
	static const char* current_item = NULL;

	ImGui::BeginGroup();
	ImGui::Text("Choose channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##videocontrolentry", current_item))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected)) {
				current_item = items[n];
				ImGui::SetItemDefaultFocus();
				if (!*viewVideo[n])
				{					
					*viewVideo[n] = true;
				}
				
			}

		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::Text("Operator:"); ImGui::SameLine();
	ImGui::Text(&nameOperator[0]); ImGui::SameLine();
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::Text(&dateDetect[0]);
	ImGui::EndGroup();
	ImGui::Separator();
	ImGui::Spacing();

}

void FrameControlEntry::ShowLastAction()
{
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
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));
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

void FrameControlEntry::ShowUnidentified(bool* menuEnroll, int& indexUnidentified)
{	
	static int hue = 140;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	ImGui::BeginChild("Not found", ImVec2(0, 180), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImFont* font_current = ImGui::GetFont();

	for (int line = 0; line < listUnidentified.size(); line++)
	{

		uint32_t textureID = listUnidentified[line].txtCapture->GetRendererID();
		ImGui::BeginGroup();
		ImGui::Text("Not found");
		ImGui::Spacing();
		ImGui::Image((void*)textureID, ImVec2{ 60.0f, 75.0f });
		ImGui::SetWindowFontScale(0.8f);
		ImGui::PushFont(font_current);
		ImGui::Text(listUnidentified[line].date); 
		ImGui::PopFont();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(col_area.x, col_area.y, col_area.z, 0.10f)); 
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
		std::string nameButton = "ENROLL-" + std::to_string(line);

		if (ImGui::Button(nameButton.c_str())) {
			if (*menuEnroll == false)
			{
				*menuEnroll = true;
				indexUnidentified = GetIndexDetected(nameButton);
			}
			
		}
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
		ImGui::EndGroup();
		if (line + 1 != listUnidentified.size())
		{
			ImGui::SameLine();
		}

	}

	ImGui::EndChild();
}

void FrameControlEntry::ShowDetected(int index, PersonSpecification& personSpec)
{	
	//static const char* current_item = NULL;
	std::string nameScreen = "Detect-" + std::to_string(index);
	ImGui::BeginChild(nameScreen.c_str(), ImVec2(0, 120), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);

	ImGui::Image((void*)listDetected[index].txtGallery->GetRendererID(), ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::Image((void*)listDetected[index].txtCapture->GetRendererID(), ImVec2{ 60.0f, 75.0f });

	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name", listDetected[index].name, IM_ARRAYSIZE(listDetected[index].name));
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##document", listDetected[index].document, IM_ARRAYSIZE(listDetected[index].document));
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##date", listDetected[index].date, IM_ARRAYSIZE(listDetected[index].date));
	ImGui::Text("Type Person:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::InputText("##type", listDetected[index].type, IM_ARRAYSIZE(listDetected[index].type),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Place:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##place", listDetected[index].place, IM_ARRAYSIZE(listDetected[index].place),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Match:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##match", listDetected[index].match, IM_ARRAYSIZE(listDetected[index].match),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("Channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputText("##channel", listDetected[index].channel, IM_ARRAYSIZE(listDetected[index].channel),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::Text("State:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##state", listDetected[index].state, IM_ARRAYSIZE(listDetected[index].state),
		ImGuiInputTextFlags_ReadOnly);
	ImGui::EndGroup();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Place:"); ImGui::SameLine();
	if (ImGui::BeginCombo("##combo", listDetected[index].place)) {
		for (int n = 0; n < IM_ARRAYSIZE(itemsTarget); n++) {
			bool is_selected = (listDetected[index].place == itemsTarget[n]);
			if (ImGui::Selectable(itemsTarget[n], is_selected)) {
				strcpy(listDetected[index].place, itemsTarget[n]);				 
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Authorize entry")) {
		indexRemove = GetIndexDetected(nameScreen);
		SetPersonInside(indexRemove, personSpec);
	}
	ImGui::Button("Authorize departure");
	ImGui::EndGroup();
	ImGui::PopFont();

	ImGui::EndChild();
}

void FrameControlEntry::SetPersonInside(int index, PersonSpecification& personSpec) {
	std::string dateDetect = ASSInterface::DateTime::Now();
	strcpy(personSpec.id, listDetected[index].id);
	strcpy(personSpec.name, listDetected[index].name);
	strcpy(personSpec.document, listDetected[index].document);
	strcpy(personSpec.id, listDetected[index].id);
	strcpy(personSpec.date, dateDetect.c_str());
	strcpy(personSpec.type, listDetected[index].type);
	personSpec.txtCapture = listDetected[index].txtCapture;
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