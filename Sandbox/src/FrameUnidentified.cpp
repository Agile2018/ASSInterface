#include "hzpch.h"
#include "FrameUnidentified.h"
#include "imgui/imgui.h"
#include <SOIL2.h>

FrameUnidentified::FrameUnidentified()
{	
}

FrameUnidentified::~FrameUnidentified()
{
}

void FrameUnidentified::Show(bool* p_open, bool* menuEnroll, int& indexUnidentified)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Unidentified Control", p_open))
	{
		ImGui::End();
		return;
	}

	SetNewsUnidentified();

	ImFont* font_current = ImGui::GetFont();
	ImGui::SetWindowFontScale(0.8f);
	ImGui::PushFont(font_current);

	for (int i = 0; i < listUnidentified.size(); i++)
	{		
				
		ImGui::Image((void*)listUnidentified[i].txtCapture,
			ImVec2{ 60.0f, 75.0f });	
		ImGui::SameLine();
		ImGui::SameLine();

		ImGui::BeginGroup();

		ImGui::Text("Channel:"); ImGui::SameLine();
		ImGui::PushItemWidth(200);
		ImGui::Text(listUnidentified[i].channel);

		ImGui::Text("Quality:"); ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::Text(listUnidentified[i].match);

		ImGui::Text("Date:"); ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::Text(listUnidentified[i].date);

		ImGui::EndGroup();

		ImGui::BeginGroup();

		ImGui::Text(std::to_string(i + 1).c_str()); ImGui::SameLine();
		std::string nameButton = "RAV-" + std::to_string(i);

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

		if (ImGui::Button(nameButton.c_str(), ImVec2(170.0f, 20.0f))) {
			if (*menuEnroll == false)
			{
				*menuEnroll = true;
				indexUnidentified = GetIndexUnidentified(nameButton);
			}

		}
		ImGui::PopStyleVar(1);

		ImGui::EndGroup();				
		
	}

	ImGui::PopFont();
	ImGui::End();

}

void FrameUnidentified::SetPersonUnidentified(PersonSpecification pSpec)
{
	listUnidentifiedTemp.push_back(pSpec);	
}

int FrameUnidentified::GetIndexUnidentified(std::string nameControl) {
	std::vector<std::string> tokens = Split(nameControl, '-');
	if (tokens.size() > 1)
	{
		return atoi(tokens[1].c_str());
	}

	return -1;
}

void FrameUnidentified::SetNewsUnidentified()
{
	std::lock_guard lock(mutexList);
	for (int i = 0; i < listUnidentifiedTemp.size(); i++)
	{
		listUnidentifiedTemp[i].txtCapture = SOIL_load_OGL_texture_from_memory(
			&listUnidentifiedTemp[i].bufferCapture[0], 
			(int)listUnidentifiedTemp[i].bufferCapture.size(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
		if (listUnidentifiedTemp[i].txtCapture != 0)
		{
			listUnidentified.push_back(listUnidentifiedTemp.at(i));			
		}
		
	}
	
	listUnidentifiedTemp.clear();
	
}

std::vector<std::string> FrameUnidentified::Split(const std::string& s, char delimiter)
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