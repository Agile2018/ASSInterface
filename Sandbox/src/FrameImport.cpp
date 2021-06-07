#include "FrameImport.h"
#include "imgui/imgui.h"
#include "ASSInterface/Utils/PlatformUtils.h"

FrameImport::FrameImport(ASSInterface::LanguageType language)
{
	lg = language;
}

FrameImport::~FrameImport()
{
}

void FrameImport::Show(bool* p_open)
{
	static const char* currentItemPersonType = NULL;
	static bool popup_file_open = false;

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment", p_open))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Import to:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##combotypeperson", currentItemPersonType)) {
		for (int n = 0; n < IM_ARRAYSIZE(ConstantApplication::TYPES_PERSON); n++) {
			bool is_selected = (currentItemPersonType == ConstantApplication::TYPES_PERSON[n]);
			if (ImGui::Selectable(ConstantApplication::TYPES_PERSON[n], is_selected)) {
				currentItemPersonType = ConstantApplication::TYPES_PERSON[n];				
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("File")) {
				
		if (currentIndexVideo == -1)
		{
			popup_file_open = true;
			ImGui::SetNextWindowPos(ImGui::GetMousePos());

		}
		else
		{
			OpenFile();
		}
		
	}

	if (popup_file_open)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("##Popupfile", &popup_file_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		if (!ImGui::IsWindowFocused())
			popup_file_open = false;

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
					CreateTaskEnroll(currentIndexVideo);
					OpenFile();

				}
				else
				{
					*messageStatus = "The channel is busy.";
				}

				popup_file_open = false;
			}

		}

		ImGui::End();
		ImGui::PopStyleVar();

	}

	ImGui::End();
}

void FrameImport::OpenFile()
{
	std::vector<std::string> filePaths = ASSInterface::FileDialogs::OpenFile(
		"Images (*.png)\0*.png\0(*.jpg)\0*.jpg\0");
	if (!filePaths.empty())
	{		
		std::thread ti(&FrameImport::BranchImport, this, filePaths);
		ti.detach();
		
		*messageStatus = "";
	}
}

void FrameImport::CreateTaskEnroll(int channel)
{
	innoTask = ASSInterface::ExecuteTask::CreateInnoTask(channel);
	innoDetect = ASSInterface::Detection::CreateInnoDetected(channel);
	innoTask->SetDatabase(dbMongo);
	ObserverDetected();
	ObserverTask();
}

void FrameImport::ObserverDetected()
{
	auto taskDetectedObservable = innoDetect->GetConcurrent()
		.map([](concurrent_vector<ASSInterface::DetectSpecification> concurrentDetection) {
			return concurrentDetection;
		});

	auto subscriptionTaskDetect = taskDetectedObservable
		.subscribe([this](concurrent_vector<ASSInterface::DetectSpecification> concurrentDetection) {
			innoTask->DoTask(concurrentDetection);
		});

 	subscriptionTaskDetect.clear();

}

void FrameImport::ObserverTask()
{
	auto taskImportObservable = innoTask->EndTask()
		.map([](int result) {
			return result;
		});

	auto subscriptionTaskImport = taskImportObservable
		.subscribe([this](int result) {
			*messageStatus = "Finish import, enrolled: " + std::to_string(result);
		});

	subscriptionTaskImport.clear();
}

void FrameImport::BranchImport(std::vector<std::string> filePaths)
{
	innoDetect->Import(filePaths);
}
