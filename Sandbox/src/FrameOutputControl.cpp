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

	ShowScreenHead();

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
	std::string firtsName = &listInside[index].name[0];
	std::string lastName = &listInside[index].lastname[0];
	std::string name = firtsName + " " + lastName;

	std::string nameScreen = "Inside-" + std::to_string(index);

	ImGui::BeginChild(nameScreen.c_str(), ImVec2(0, 150), true);
	ImFont* font_current = ImGui::GetFont();

	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);

	ImGui::Image((void*)listInside[index].txtGallery, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();
	ImGui::Image((void*)listInside[index].txtCapture, ImVec2{ 60.0f, 75.0f });
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("Name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::Text(name.c_str());

	ImGui::Text("C.I.:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listInside[index].document);	

	ImGui::Text("Channel:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::Text(listInside[index].channel);

	ImGui::Text("Match:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::Text(listInside[index].match);

	ImGui::Text("Date:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	ImGui::Text(listInside[index].date);

	ImGui::Text("Gallery:"); ImGui::SameLine();
	ImGui::PushItemWidth(120);
	ImGui::Text(listInside[index].type);
		
	ImGui::EndGroup();

	ImGui::BeginGroup();
	
	ImGui::Text(std::to_string(index + 1).c_str()); ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Button("Document", ImVec2(100.0f, 20.0f))) {
		
	}
	ImGui::SameLine();
	if (ImGui::Button("Inside out", ImVec2(100.0f, 20.0f))) {
		indexRemove = GetIndexOut(nameScreen);
		SaveEventOutside(indexRemove);
	}
	ImGui::PopStyleVar(1);
	ImGui::EndGroup();

	ImGui::PopFont();

	ImGui::EndChild();
}

void FrameOutputControl::ShowScreenHead()
{
	ImGui::SetCursorPos(ImVec2(10.0f, 30.0f));
	ImGui::Text("Registered"); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(90.0f, 30.0f));
	ImGui::Text("Captured"); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(220.0f, 30.0f));
	ImGui::Text("Data"); 
	ImGui::Separator();
	ImGui::Spacing();
}

int FrameOutputControl::GetIndexOut(std::string nameControl) {
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

void FrameOutputControl::SaveEventOutside(int index)
{
	ASSInterface::EntityEvent entEvent;
	entEvent.id = &listInside[index].id[0];
	entEvent.date = ASSInterface::DateTime::NowToLong();
	entEvent.type = ASSInterface::EventAppType::outside;
	entEvent.description = "The person leaves the premises.";
	int id = atoi(entEvent.id.c_str());

	dbMongo->Add(entEvent);

	shootPersonOutside.on_next(id);
}
