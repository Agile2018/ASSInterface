#include "hzpch.h"
#include "FrameOutputControl.h"
#include "imgui/imgui.h"

FrameOutputControl::FrameOutputControl()
{
}

FrameOutputControl::~FrameOutputControl()
{
}

void FrameOutputControl::Show(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Output Control", p_open))
	{
		ImGui::End();
		return;
	}

	if (indexRemove != -1) {
		RemovePersonInside(indexRemove);
		indexRemove = -1;
	}

	for (int i = 0; i < listInside.size(); i++)
	{
		ShowPersonsInside(i);
	}

	ImGui::End();
}

void FrameOutputControl::ShowPersonsInside(int index) {	
	std::string dateOut = ASSInterface::DateTime::Now();
	strcpy(listInside[index].date, dateOut.c_str());

	std::string nameScreen = "Inside-" + std::to_string(index);

	ImGui::BeginChild(nameScreen.c_str(), ImVec2(0, 140), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);
	ImGui::Image((void*)listInside[index].txtCapture->GetRendererID(), ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name", listInside[index].name, IM_ARRAYSIZE(listInside[index].name),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##document", listInside[index].document, IM_ARRAYSIZE(listInside[index].document),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::InputText("##date", listInside[index].date, IM_ARRAYSIZE(listInside[index].date),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::Text("Type Person:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::InputText("##type", listInside[index].type, IM_ARRAYSIZE(listInside[index].type),
		ImGuiInputTextFlags_AllowTabInput | (typePersonReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));	
	if (ImGui::Button("Inside out")) {
		indexRemove = GetIndexUnidentified(nameScreen);
	}	
	ImGui::Spacing();
	ImGui::EndGroup();
	ImGui::PopFont();

	ImGui::EndChild();
}

int FrameOutputControl::GetIndexUnidentified(std::string nameControl) {
	std::vector<std::string> tokens = Split(nameControl, '-');
	if (tokens.size() > 1)
	{
		return atoi(tokens[1].c_str());
	}

	return -1;
}

std::vector<std::string> FrameOutputControl::Split(const std::string& s, char delimiter)
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