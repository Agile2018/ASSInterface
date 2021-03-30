#include "hzpch.h"
#include "FrameUnidentified.h"
#include "imgui/imgui.h"

FrameUnidentified::FrameUnidentified()
{
}

FrameUnidentified::~FrameUnidentified()
{
}

void FrameUnidentified::Show(bool* p_open, bool* menuEnroll, int& indexUnidentified)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Control Unidentified", p_open))
	{
		ImGui::End();
		return;
	}

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
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));
		std::string nameButton = "ENROLL-" + std::to_string(line);

		if (ImGui::Button(nameButton.c_str())) {
			if (*menuEnroll == false)
			{
				*menuEnroll = true;
				indexUnidentified = GetIndexUnidentified(nameButton);
			}

		}
		ImGui::PopStyleVar(1);		
		ImGui::EndGroup();
		
	}

	ImGui::End();
}

int FrameUnidentified::GetIndexUnidentified(std::string nameControl) {
	std::vector<std::string> tokens = Split(nameControl, '-');
	if (tokens.size() > 1)
	{
		return atoi(tokens[1].c_str());
	}

	return -1;
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