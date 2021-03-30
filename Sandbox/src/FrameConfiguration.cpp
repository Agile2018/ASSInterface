#include "hzpch.h"
#include "FrameConfiguration.h"
#include "imgui/imgui.h"

FrameConfiguration::FrameConfiguration(ASSInterface::LanguageType language)
{
	lg = language;
	SetParametersDatabase();
	SetParametersGlobals();
	configurationChannel = ASSInterface::Configuration::CreateConfigVideo("video.txt");
	configurationFace = ASSInterface::Configuration::CreateConfigFace();
	configurationTrack = ASSInterface::Configuration::CreateConfigTrack();
	configurationEnroll = ASSInterface::Configuration::CreateConfigEnroll();
	configurationEntry = ASSInterface::Configuration::CreateConfigEntry();
	configurationOnTheFly = ASSInterface::Configuration::CreateConfigOnTheFly();
}

FrameConfiguration::~FrameConfiguration()
{
}

void FrameConfiguration::ShowParametersFaceProcessing(bool* p_open)
{
	
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);


	if (!ImGui::Begin(ASSInterface::Label::GetLabel(LBL_FACE_PROCESSING, lg).c_str(), p_open))
	{
		ImGui::End();
		return;
	}
	SetParametersFace();

	ImGui::Text(ASSInterface::Label::GetLabel(LBL_CHANNEL_NUMBER, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	ImGui::Separator();
	ImGui::Spacing();

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt(ASSInterface::Label::GetLabel(LBL_CONFIDENCE_THRESHOLD, lg).c_str(), &confidenceThreshold, 0, 10000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_MAX_IMAGE_SIZE, lg));
	ImGui::PushItemWidth(150);	
	ImGui::SliderFloat(ASSInterface::Label::GetLabel(LBL_MAX_IMAGE_SIZE, lg).c_str(), &maxFace, 0.0f, 400.0f);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_MIN_IMAGE_SIZE, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderFloat(ASSInterface::Label::GetLabel(LBL_MIN_IMAGE_SIZE, lg).c_str(), &minFace, 0.0f, 25.0f);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_QUALITY_TEMPLATE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt(ASSInterface::Label::GetLabel(LBL_QUALITY_TEMPLATE, lg).c_str(), &qualityModel, 1, 255);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_MAXIMUM_FACES, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt(ASSInterface::Label::GetLabel(LBL_MAXIMUM_FACES, lg).c_str(), &maxDetected, 1, 5);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_DET_SPEED_ACCURACY, lg));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_DET_SPEED_ACCURACY, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##speeddetect", speedDetectDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(speedDetectExtractionMode); n++) {
			bool is_selected = (speedDetectDesc == speedDetectExtractionMode[n]);
			if (ImGui::Selectable(speedDetectExtractionMode[n], is_selected)) {
				speedDetectDesc = speedDetectExtractionMode[n];
				ImGui::SetItemDefaultFocus();
				speedDetect = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_TEMPLATE_SPEED, lg));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_TEMPLATE_SPEED, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##speedextraction", speedExtractionDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(speedDetectExtractionMode); n++) {
			bool is_selected = (speedExtractionDesc == speedDetectExtractionMode[n]);
			if (ImGui::Selectable(speedDetectExtractionMode[n], is_selected)) {
				speedExtractionDesc = speedDetectExtractionMode[n];
				ImGui::SetItemDefaultFocus();
				speedExtraction = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_FACE_CROP, lg));
	ImGui::Text(ASSInterface::Label::GetLabel(LBL_FACE_CROP, lg).c_str()); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##facecrop", faceCropDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(crop); n++) {
			bool is_selected = (faceCropDesc == crop[n]);
			if (ImGui::Selectable(crop[n], is_selected)) {
				faceCropDesc = crop[n];
				ImGui::SetItemDefaultFocus();
				faceCrop = n;
			}
		}
		ImGui::EndCombo();
	}


	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::Text("Face Detection Forced:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##detectforced", detectForcedDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(yesOrno); n++) {
			bool is_selected = (detectForcedDesc == yesOrno[n]);
			if (ImGui::Selectable(yesOrno[n], is_selected)) {
				detectForcedDesc = yesOrno[n];
				ImGui::SetItemDefaultFocus();
				detectForced = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::Text("Face detection ignore multiple faces:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##multipleface", multipleFaceDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(yesOrno); n++) {
			bool is_selected = (multipleFaceDesc == yesOrno[n]);
			if (ImGui::Selectable(yesOrno[n], is_selected)) {
				multipleFaceDesc = yesOrno[n];
				ImGui::SetItemDefaultFocus();
				multipleFace = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::Text("Face detection:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##detected", detectedDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(modeDetected); n++) {
			bool is_selected = (detectedDesc == modeDetected[n]);
			if (ImGui::Selectable(modeDetected[n], is_selected)) {
				detectedDesc = modeDetected[n];
				ImGui::SetItemDefaultFocus();
				detected = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::Text("Face extraction:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##extraction", extractionDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(modeExtraction); n++) {
			bool is_selected = (extractionDesc == modeExtraction[n]);
			if (ImGui::Selectable(modeExtraction[n], is_selected)) {
				extractionDesc = modeExtraction[n];
				ImGui::SetItemDefaultFocus();
				extraction = n;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersFace();
	}
	ImGui::End();
}

void FrameConfiguration::ShowParametersFaceTracking(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Tracking processing", p_open))
	{
		ImGui::End();
		return;
	}

	SetParametersTrack();

	ImGui::Text("Channel Number:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();

	PutToolTip(ASSInterface::Label::GetLabel(DESC_TRACK_DISCOVERY_FREQUENCE, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Track Discovery Frequence:", &discoveryFrecuence, 100, 3000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_REDETECT_TIME_DELTA, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Time tracking an object:", &timeRedetect, 0, 10000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_DEEP_TRACKING, lg));
	ImGui::Checkbox("Deep Tracking", &deepTracking);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_DEEP_TRACKING, lg));
	ImGui::Text("Track Tracking:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##tracking", trackingDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(modeTracking); n++) {
			bool is_selected = (trackingDesc == modeTracking[n]);
			if (ImGui::Selectable(modeTracking[n], is_selected)) {
				trackingDesc = modeTracking[n];
				ImGui::SetItemDefaultFocus();
				tracking = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_DEEP_TRACKING, lg));
	ImGui::Text("Track speed and accuracy:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##trackspeed", speedTrackDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(speedTracking); n++) {
			bool is_selected = (speedTrackDesc == speedTracking[n]);
			if (ImGui::Selectable(speedTracking[n], is_selected)) {
				speedTrackDesc = speedTracking[n];
				ImGui::SetItemDefaultFocus();
				speedTrack = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_DEEP_TRACKING, lg));
	ImGui::Text("Track motion optimization:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##trackoptimus", trackOptimusDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(optimizationTracking); n++) {
			bool is_selected = (trackOptimusDesc == optimizationTracking[n]);
			if (ImGui::Selectable(optimizationTracking[n], is_selected)) {
				trackOptimusDesc = optimizationTracking[n];
				ImGui::SetItemDefaultFocus();
				trackOptimus = n;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersTrack();
	}
	ImGui::End();
}

void FrameConfiguration::ShowParametersEnrollmentProcessing(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enrollment processing", p_open))
	{
		ImGui::End();
		return;
	}
	SetParametersEnroll();
	ImGui::Text("Channel Number:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();

	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::Text("Gallery name:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	if (ImGui::BeginCombo("##gallery", galleryDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(galleryOptions); n++) {
			bool is_selected = (galleryDesc == galleryOptions[n]);
			if (ImGui::Selectable(galleryOptions[n], is_selected)) {
				galleryDesc = galleryOptions[n];
				ImGui::SetItemDefaultFocus();
				gallery = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Face detection threshold on database:", &detectionThreshold, 0, 10000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::Text("Deduplicate enrollment:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##deduplicate", deduplicateDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(yesOrno); n++) {
			bool is_selected = (deduplicateDesc == yesOrno[n]);
			if (ImGui::Selectable(yesOrno[n], is_selected)) {
				deduplicateDesc = yesOrno[n];
				ImGui::SetItemDefaultFocus();
				deduplicate = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Deduplication similarity threshold:", &similarityThreshold, 0, 1000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::Text("Concatenate enrollment templates:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##concatenate", concatenateDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(yesOrno); n++) {
			bool is_selected = (concatenateDesc == yesOrno[n]);
			if (ImGui::Selectable(yesOrno[n], is_selected)) {
				concatenateDesc = yesOrno[n];
				ImGui::SetItemDefaultFocus();
				concatenate = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Maximun Concatenated Templates:", &maxTemplates, -1, 100);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::Text("Concatenation mode:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##concatenatemode", concatenateModeDesc)) {
		for (int n = 0; n < IM_ARRAYSIZE(concatenateOptions); n++) {
			bool is_selected = (concatenateModeDesc == concatenateOptions[n]);
			if (ImGui::Selectable(concatenateOptions[n], is_selected)) {
				concatenateModeDesc = concatenateOptions[n];
				ImGui::SetItemDefaultFocus();
				concatenateMode = n;
			}
		}
		ImGui::EndCombo();
	}
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Enroll Concatenation minimum score threshold:", &minScore, 0, 100);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_GALLERY, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Enroll Concatenation maximum score threshold:", &maxScore, 0, 100);

	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersEnroll();
	}

	ImGui::End();
}

void FrameConfiguration::ShowParametersControlEntryMatch(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Access control and matching", p_open))
	{
		ImGui::End();
		return;
	}
	SetParametersEntry();
	ImGui::Text("Channel Number:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Identification similarity threshold:", &idenSimilarityThreshold, 0, 1000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::PushItemWidth(150);
	ImGui::SliderInt("Verification similarity threshold:", &verySimilarityThreshold, 0, 1000);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Best candidates count:", &bestCadidates, 1, 5);
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CONFIDENCE_THRESHOLD, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Identification speed:", &identificationSpeed, 0, 10);
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersEntry();
	}
	ImGui::End();
}

void FrameConfiguration::ShowParametersChannel(bool* p_open)
{		

	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Channel parameters", p_open))
	{
		ImGui::End();
		return;
	}
	SetParametersChannel();
	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_NUMBER, lg));
	ImGui::Text("Channel Number:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_ADDRESS, lg));
	ImGui::Text("Channel address:"); ImGui::SameLine();
	ImGui::PushItemWidth(400);
	ImGui::InputText("##address", channelAddress, IM_ARRAYSIZE(channelAddress));

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_NAME, lg));
	ImGui::Text("Channel name:"); ImGui::SameLine();
	ImGui::PushItemWidth(200);
	ImGui::InputText("##namechannel", channelName, IM_ARRAYSIZE(channelName));

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::Text("Channel type:"); ImGui::SameLine();
	ImGui::RadioButton("IP", &channelType, 1); ImGui::SameLine();
	ImGui::RadioButton("FILE", &channelType, 2); ImGui::SameLine();
	ImGui::RadioButton("DEVICE", &channelType, 3);
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersChannel();
	}

	ImGui::End();
}

void FrameConfiguration::ShowParametersBiometric(bool* p_open)
{		
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Biometric parameters", p_open))
	{
		ImGui::End();
		return;
	}

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Face Engine Log level:", &log, -1, 3);

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::Text("Global GPU enabled:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##gpu", cgpu)) {
		for (int n = 0; n < IM_ARRAYSIZE(yesOrno); n++) {
			bool is_selected = (cgpu == yesOrno[n]);
			if (ImGui::Selectable(yesOrno[n], is_selected)) {
				cgpu = yesOrno[n];
				ImGui::SetItemDefaultFocus();
				gpu = n;
			}
		}
		ImGui::EndCombo();
	}

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("GPU device ID:", &id, 0, 3);

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::Text("Thread management mode:"); ImGui::SameLine();
	ImGui::PushItemWidth(100);
	if (ImGui::BeginCombo("##thread_mode", threadManagement)) {
		for (int n = 0; n < IM_ARRAYSIZE(multithreadingMode); n++) {
			bool is_selected = (threadManagement == multithreadingMode[n]);
			if (ImGui::Selectable(multithreadingMode[n], is_selected)) {
				threadManagement = multithreadingMode[n];
				ImGui::SetItemDefaultFocus();
				mode = n;
			}
		}
		ImGui::EndCombo();
	}

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Thread Number:", &thrNumber, 1, 12);

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Min valid image size:", &sizeImg, 15, 400);
	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		
		SaveParameteresGlobals();
	}
	ImGui::End();
}

void FrameConfiguration::ShowParametersOnthefly(bool* p_open)
{
	
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Enroll on the fly", p_open))
	{
		ImGui::End();
		return;
	}

	SetParametersOnTheFly();
	ImGui::Text("Channel Number:"); ImGui::SameLine();
	ImGui::PushItemWidth(50);
	if (ImGui::BeginCombo("##channel", channel)) {
		for (int n = 0; n < IM_ARRAYSIZE(numberChannels); n++) {
			bool is_selected = (channel == numberChannels[n]);
			if (ImGui::Selectable(numberChannels[n], is_selected)) {
				channel = numberChannels[n];
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();
	ImGui::Spacing();

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Maximum number of templates:", &numberTemplates, 1, 15);

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Show maximum number of images:", &showNumberImages, 1, 10);

	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersOnTheFly();
	}
	ImGui::End();
}

void FrameConfiguration::ShowParametersDatabase(bool* p_open)
{
	
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Configuration database", p_open))
	{
		ImGui::End();
		return;
	}

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::Text("Name database:"); ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText("##name_database", databaseName, IM_ARRAYSIZE(databaseName));

	PutToolTip(ASSInterface::Label::GetLabel(DESC_CHANNEL_TYPE, lg));
	ImGui::Text("Connect string:"); ImGui::SameLine();
	ImGui::PushItemWidth(400);
	ImGui::InputText("##connect_database", connectString, IM_ARRAYSIZE(connectString));

	ImGui::Separator();
	ImGui::Spacing();
	if (ImGui::Button("Save parameters")) {
		SaveParametersDatabase();
	}
	ImGui::SameLine();

	if (ImGui::Button("Delete Database"))
		ImGui::OpenPopup("Delete?");
	if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Text("All collections will be deleted.\nThis operation cannot be undone!\nAre you sure?\n\n");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) { 
			DropDatabase();
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::End();
}

void FrameConfiguration::PutToolTip(std::string description)
{
	ImGui::TextDisabled("(?)"); ImGui::SameLine();
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(description.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void FrameConfiguration::SetParametersDatabase() {
	configurationDB = ASSInterface::Configuration::CreateConfigDatabase();
	configurationDB->ParseToObject();

	std::any anyCnn = configurationDB->GetParam("connect");
	std::any anyDb = configurationDB->GetParam("name");
	if (anyCnn.has_value() && anyDb.has_value()) {
		std::string cnn = std::any_cast<std::string>(anyCnn);
		std::string db = std::any_cast<std::string>(anyDb);
		strcpy(databaseName, db.c_str());
		strcpy(connectString, cnn.c_str());
	}
}

void FrameConfiguration::SetParametersEntry() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel) {
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "entry" + numberChannel + ".txt";
		configurationEntry->SetPath(nameFile);
		configurationEntry->ParseToObject();

		std::any anyIdenSimilarity = configurationEntry->GetParam("CFG_IDENTIFICATION_SIMILARITY_THRESHOLD");
		std::any anyVeriSimilarity = configurationEntry->GetParam("CFG_VERIFICATION_SIMILARITY_THRESHOLD");
		std::any anyBestCandidates = configurationEntry->GetParam("CFG_BEST_CANDIDATES_COUNT");
		std::any anyIdentificationSpeed = configurationEntry->GetParam("CFG_IDENTIFICATION_SPEED");

		idenSimilarityThreshold = std::any_cast<int>(anyIdenSimilarity);
		verySimilarityThreshold = std::any_cast<int>(anyVeriSimilarity);
		bestCadidates = std::any_cast<int>(anyBestCandidates);
		identificationSpeed = std::any_cast<int>(anyIdentificationSpeed);
	}
}

void FrameConfiguration::SetParametersFace() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel) {
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "face" + numberChannel + ".txt";
		configurationFace->SetPath(nameFile);
		configurationFace->ParseToObject();

		std::any anyMaxDetect = configurationFace->GetParam("FACE_MAX_DETECT");
		std::any anyMinFace = configurationFace->GetParam("TRACK_MIN_FACE_SIZE");
		std::any anyMaxFace = configurationFace->GetParam("TRACK_MAX_FACE_SIZE");
		std::any anyConfidence = configurationFace->GetParam("FACEDET_CONFIDENCE_THRESHOLD");
		std::any anySpeedDetect = configurationFace->GetParam("FACEDET_SPEED_ACCURACY_MODE");
		std::any anySpeedTemplate = configurationFace->GetParam("FACETMPLEXT_SPEED_ACCURACY_MODE");
		std::any anyCrop = configurationFace->GetParam("IFACE_CROP_IMAGE");
		std::any anyModel = configurationFace->GetParam("QUALITY_MODEL");
		std::any anyDetection = configurationFace->GetParam("CFG_IFACE_DETECTION_MODE");
		std::any anyExtraction = configurationFace->GetParam("CFG_IFACE_EXTRACTION_MODE");
		std::any anyForced = configurationFace->GetParam("CFG_IFACE_DETECT_FORCED");
		std::any anyMultipleFaces = configurationFace->GetParam("CFG_IFACE_IGNORE_MULTIPLE_FACES");
		
		maxDetected = std::any_cast<int>(anyMaxDetect);
		minFace = std::any_cast<float>(anyMinFace);
		maxFace = std::any_cast<float>(anyMaxFace);
		confidenceThreshold = std::any_cast<int>(anyConfidence);
		speedDetect = std::any_cast<int>(anySpeedDetect);
		speedExtraction = std::any_cast<int>(anySpeedTemplate);
		faceCrop = std::any_cast<int>(anyCrop);
		qualityModel = std::any_cast<int>(anyModel);
		detected = std::any_cast<int>(anyDetection);
		extraction = std::any_cast<int>(anyExtraction);
		detectForced = std::any_cast<int>(anyForced);
		multipleFace = std::any_cast<int>(anyMultipleFaces);

		speedDetectDesc = speedDetectExtractionMode[speedDetect];
		speedExtractionDesc = speedDetectExtractionMode[speedExtraction];
		faceCropDesc = crop[faceCrop];
		multipleFaceDesc = yesOrno[multipleFace];
		detectedDesc = modeDetected[detected];
		extractionDesc = modeExtraction[extraction];
		detectForcedDesc = yesOrno[detectForced];
	}
	
}

void FrameConfiguration::SetParametersTrack() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel) {
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "track" + numberChannel + ".txt";
		configurationTrack->SetPath(nameFile);
		configurationTrack->ParseToObject();

		std::any anyDiscovery = configurationTrack->GetParam("TRACK_FACE_DISCOVERY_FREQUENCE_MS");
		std::any anyTimeDelta = configurationTrack->GetParam("COUNT_REDETECT_TIME_DELTA");
		std::any anyTracking = configurationTrack->GetParam("TRACK_TRACKING_MODE");
		std::any anySpeedTrack = configurationTrack->GetParam("TRACK_SPEED_ACCURACY_MODE");
		std::any anyOptimus = configurationTrack->GetParam("TRACK_MOTION_OPTIMIZATION");
		std::any anyDeep = configurationTrack->GetParam("TRACK_DEEP_TRACK");

		discoveryFrecuence = std::any_cast<int>(anyDiscovery);
		timeRedetect = std::any_cast<int>(anyTimeDelta);
		tracking = std::any_cast<int>(anyTracking);
		speedTrack = std::any_cast<int>(anySpeedTrack);
		trackOptimus = std::any_cast<int>(anyOptimus);
		deepTracking = std::any_cast<bool>(anyDeep);

		trackingDesc = modeTracking[tracking];
		speedTrackDesc = speedTracking[speedTrack];
		trackOptimusDesc = optimizationTracking[trackOptimus];

	}

}

void FrameConfiguration::SetParametersEnroll() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel) {
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "enroll" + numberChannel + ".txt";
		configurationEnroll->SetPath(nameFile);
		configurationEnroll->ParseToObject();

		std::any anyDetectThreshold = configurationEnroll->GetParam("CFG_IFACE_DETECTION_THRESHOLD");
		std::any anySimilarityThreshold = configurationEnroll->GetParam("CFG_SIMILARITY_THRESHOLD");
		std::any anyGallery = configurationEnroll->GetParam("AFACE_GALLERY");
		std::any anyDeduplicate = configurationEnroll->GetParam("AFACE_DEDUPLICATE");
		std::any anyConcatenate = configurationEnroll->GetParam("AFACE_CONCATENATE");
		std::any anyMaxTemplates = configurationEnroll->GetParam("AFACE_MAX_TEMPLATES");
		std::any anyConcatenateMode = configurationEnroll->GetParam("AFACE_CONCATENATE_MODE");
		std::any anyMinScore = configurationEnroll->GetParam("AFACE_MIN_SCORE");
		std::any anyMaxScore = configurationEnroll->GetParam("AFACE_MAX_SCORE");

		detectionThreshold = std::any_cast<int>(anyDetectThreshold);
		similarityThreshold = std::any_cast<int>(anySimilarityThreshold);
		gallery = std::any_cast<int>(anyGallery);
		deduplicate = std::any_cast<int>(anyDeduplicate);
		concatenate = std::any_cast<int>(anyConcatenate);
		maxTemplates = std::any_cast<int>(anyMaxTemplates);
		concatenateMode = std::any_cast<int>(anyConcatenateMode);
		minScore = std::any_cast<int>(anyMinScore);
		maxScore = std::any_cast<int>(anyMaxScore);
		galleryDesc = galleryOptions[gallery];
		deduplicateDesc = yesOrno[deduplicate];
		concatenateDesc = yesOrno[concatenate];
		concatenateModeDesc = concatenateOptions[concatenateMode];

	}
}

void FrameConfiguration::SaveParametersEnroll() {
	if (channel != nullptr) {
		std::string numberChannel = channel;
		std::string nameFile = "enroll" + numberChannel + ".txt";
		configurationEnroll->SetPath(nameFile);

		configurationEnroll->SetParam("CFG_IFACE_DETECTION_THRESHOLD", detectionThreshold);
		configurationEnroll->SetParam("CFG_SIMILARITY_THRESHOLD", similarityThreshold);
		configurationEnroll->SetParam("AFACE_GALLERY", gallery);
		configurationEnroll->SetParam("AFACE_DEDUPLICATE", deduplicate);
		configurationEnroll->SetParam("AFACE_CONCATENATE", concatenate);
		configurationEnroll->SetParam("AFACE_MAX_TEMPLATES", maxTemplates);
		configurationEnroll->SetParam("AFACE_CONCATENATE_MODE", concatenateMode);
		configurationEnroll->SetParam("AFACE_MIN_SCORE", minScore);
		configurationEnroll->SetParam("AFACE_MAX_SCORE", maxScore);

		configurationEnroll->ParseToFile();
	}
}

void FrameConfiguration::SaveParametersTrack() {
	if (channel != nullptr) {
		std::string numberChannel = channel;
		std::string nameFile = "track" + numberChannel + ".txt";
		configurationTrack->SetPath(nameFile);

		configurationTrack->SetParam("TRACK_FACE_DISCOVERY_FREQUENCE_MS", discoveryFrecuence);
		configurationTrack->SetParam("COUNT_REDETECT_TIME_DELTA", timeRedetect);
		configurationTrack->SetParam("TRACK_TRACKING_MODE", tracking);
		configurationTrack->SetParam("TRACK_SPEED_ACCURACY_MODE", speedTrack);
		configurationTrack->SetParam("TRACK_MOTION_OPTIMIZATION", trackOptimus);
		configurationTrack->SetParam("TRACK_DEEP_TRACK", deepTracking);		

		configurationTrack->ParseToFile();
	}

}

void FrameConfiguration::SetParametersChannel() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel)
	{
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "video" + numberChannel + ".txt";
		configurationChannel->SetPath(nameFile);
		configurationChannel->ParseToObject();
		std::any anySource = configurationChannel->GetParam("sourceFlow");	
		std::any anyLbl = configurationChannel->GetParam("label");
		std::string lblWindow = std::any_cast<std::string>(anyLbl);
		strcpy(channelName, lblWindow.c_str());
		int src = std::any_cast<int>(anySource);
		channelType = src;
		switch (src)
		{
		case 1: {
			std::any anyAddress = configurationChannel->GetParam("ipCamera");
			std::string address = std::any_cast<std::string>(anyAddress);
			strcpy(channelAddress, address.c_str());
			break;
		}			
		case 2: {
			std::any anyAddress = configurationChannel->GetParam("fileVideo");
			std::string address = std::any_cast<std::string>(anyAddress);
			strcpy(channelAddress, address.c_str());
			break;
		}
			
		case 3: {
			std::any anyAddress = configurationChannel->GetParam("deviceVideo");
			std::string address = std::any_cast<std::string>(anyAddress);
			strcpy(channelAddress, address.c_str());
			break;
		}			
		default:
			break;
		}

	}
}

void FrameConfiguration::SaveParametersDatabase() {
	std::string db = &databaseName[0];
	std::string cnn = &connectString[0];
	configurationDB->SetParam("name", db);
	configurationDB->SetParam("connect", cnn);
	configurationDB->ParseToFile();
}

void FrameConfiguration::SaveParametersFace() {
	if (channel != nullptr) {
		std::string numberChannel = channel;
		std::string nameFile = "face" + numberChannel + ".txt";
		configurationFace->SetPath(nameFile);

		configurationFace->SetParam("FACE_MAX_DETECT", maxDetected);
		configurationFace->SetParam("TRACK_MIN_FACE_SIZE", minFace);
		configurationFace->SetParam("TRACK_MAX_FACE_SIZE", maxFace);
		configurationFace->SetParam("FACEDET_CONFIDENCE_THRESHOLD", confidenceThreshold);
		configurationFace->SetParam("FACEDET_SPEED_ACCURACY_MODE", speedDetect);
		configurationFace->SetParam("FACETMPLEXT_SPEED_ACCURACY_MODE", speedExtraction);
		configurationFace->SetParam("IFACE_CROP_IMAGE", faceCrop);
		configurationFace->SetParam("QUALITY_MODEL", qualityModel);
		configurationFace->SetParam("CFG_IFACE_DETECTION_MODE", detected);
		configurationFace->SetParam("CFG_IFACE_EXTRACTION_MODE", extraction);
		configurationFace->SetParam("CFG_IFACE_DETECT_FORCED", detectForced);
		configurationFace->SetParam("CFG_IFACE_IGNORE_MULTIPLE_FACES", multipleFace);

		configurationFace->ParseToFile();
	}
}

void FrameConfiguration::SaveParametersEntry() {
	if (channel != nullptr) {
		std::string numberChannel = channel;
		std::string nameFile = "entry" + numberChannel + ".txt";
		configurationEntry->SetPath(nameFile);

		configurationEntry->SetParam("CFG_IDENTIFICATION_SIMILARITY_THRESHOLD", idenSimilarityThreshold);
		configurationEntry->SetParam("CFG_VERIFICATION_SIMILARITY_THRESHOLD", verySimilarityThreshold);
		configurationEntry->SetParam("CFG_BEST_CANDIDATES_COUNT", bestCadidates);
		configurationEntry->SetParam("CFG_IDENTIFICATION_SPEED", identificationSpeed);
		
		configurationEntry->ParseToFile();
	}
}

void FrameConfiguration::SaveParametersChannel() {
	if (channel != nullptr) {		
		std::string numberChannel = channel;
		std::string nameFile = "video" + numberChannel + ".txt";
		configurationChannel->SetPath(nameFile);

		std::string address = &channelAddress[0];
		std::string lblWindow = &channelName[0];
		lblWindow = "Video " + lblWindow;
		configurationChannel->SetParam("sourceFlow", channelType);
		configurationChannel->SetParam("label", lblWindow);
		switch (channelType)
		{
		case 1: {
			configurationChannel->SetParam("ipCamera", address);
			break;
		}
		case 2: {
			configurationChannel->SetParam("fileVideo", address);
			break;
		}
		case 3 : {
			configurationChannel->SetParam("deviceVideo", address);
			break;
		}
		default:
			break;
		}
		
		configurationChannel->ParseToFile();
	}
}

void FrameConfiguration::SaveParameteresGlobals() 
{
	configurationGlobals->SetParam("GLOBAL_CFG_LOG_LEVEL", log);
	configurationGlobals->SetParam("GLOBAL_GPU_ENABLED", gpu);
	configurationGlobals->SetParam("GLOBAL_GPU_DEVICE_ID", id);
	configurationGlobals->SetParam("GLOBAL_THREAD_MANAGEMENT_MODE", mode);
	configurationGlobals->SetParam("GLOBAL_THREAD_NUM", thrNumber);
	configurationGlobals->SetParam("GLOBAL_MIN_VALID_IMAGE_SIZE", sizeImg);
	configurationGlobals->ParseToFile();
}

void FrameConfiguration::SetParametersGlobals() {
	configurationGlobals = ASSInterface::Configuration::CreateConfigGlobal();
	configurationGlobals->ParseToObject();

	std::any anyLog = configurationGlobals->GetParam("GLOBAL_CFG_LOG_LEVEL");
	std::any anyGpu = configurationGlobals->GetParam("GLOBAL_GPU_ENABLED");
	std::any anyId = configurationGlobals->GetParam("GLOBAL_GPU_DEVICE_ID");
	std::any anyMode = configurationGlobals->GetParam("GLOBAL_THREAD_MANAGEMENT_MODE");
	std::any anyNum = configurationGlobals->GetParam("GLOBAL_THREAD_NUM");
	std::any anySize = configurationGlobals->GetParam("GLOBAL_MIN_VALID_IMAGE_SIZE");
	log = std::any_cast<int>(anyLog);
	id = std::any_cast<int>(anyId);
	gpu = std::any_cast<int>(anyGpu);
	mode = std::any_cast<int>(anyMode);
	thrNumber = std::any_cast<int>(anyNum);
	sizeImg = std::any_cast<int>(anySize);
	cgpu = yesOrno[gpu];
	threadManagement = multithreadingMode[mode];
}

void FrameConfiguration::SetParametersOnTheFly() {
	static char chn = '0';

	if (channel != nullptr && chn != *channel) {
		chn = *channel;
		std::string numberChannel = channel;
		std::string nameFile = "rav" + numberChannel + ".txt";
		configurationOnTheFly->SetPath(nameFile);
		configurationOnTheFly->ParseToObject();

		std::any anyTemplates = configurationOnTheFly->GetParam("AFACE_NUMBER_TEMPLATES");
		std::any anyImages= configurationOnTheFly->GetParam("AFACE_NUMBER_IMAGES");

		numberTemplates = std::any_cast<int>(anyTemplates);
		showNumberImages = std::any_cast<int>(anyImages);
	}
}

void FrameConfiguration::SaveParametersOnTheFly() {
	if (channel != nullptr) {
		std::string numberChannel = channel;
		std::string nameFile = "rav" + numberChannel + ".txt";
		configurationOnTheFly->SetPath(nameFile);

		configurationOnTheFly->SetParam("AFACE_NUMBER_TEMPLATES", numberTemplates);
		configurationOnTheFly->SetParam("AFACE_NUMBER_IMAGES", showNumberImages);

		configurationOnTheFly->ParseToFile();
	}
}

void FrameConfiguration::DropDatabase() {
	std::string db = &databaseName[0];
	std::string cnn = &connectString[0];

	if (!db.empty() && !cnn.empty())
	{
		dbMongo = ASSInterface::Database::Create(cnn);
		dbMongo->SetDatabase(db);
		dbMongo->Drop();
	}
}