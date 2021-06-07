#include "hzpch.h"

#include "InnoIdentification.h"

namespace ASSInterface {
	InnoIdentification::InnoIdentification(int channel)
	{		
		indexChannel = channel;
		errorIDkit = ASSInterface::ErrorIdentification::CreateIDkit();
		GetParamsDetectFace();
		GetParamsEnroll();
		GetParamsControlEntry();
	}

	InnoIdentification::~InnoIdentification()
	{
	}

	void InnoIdentification::ResetParameters()
	{
		CloseConnection();
		GetParamsDetectFace();
		GetParamsEnroll();
		GetParamsControlEntry();
	}

	void InnoIdentification::SetParameters(int option)
	{
		int errorCode = IENGINE_E_NOERROR;

		switch (option)
		{
		case SIMILARITY_THRESHOLD_IDENTIFICATION:
			errorCode = IEngine_SetParameter(CFG_SIMILARITY_THRESHOLD,
				identificationSimilarityThreshold);
			CheckError("InnoIdentification::SetParameters", errorCode);
			break;
		case SIMILARITY_THRESHOLD_DEDUPLICATION:
			errorCode = IEngine_SetParameter(CFG_SIMILARITY_THRESHOLD,
				deduplicationSimilarityThreshold);
			CheckError("InnoIdentification::SetParameters", errorCode);
			break;
		case SIMILARITY_THRESHOLD_VERIFICATION:
			errorCode = IEngine_SetParameter(CFG_SIMILARITY_THRESHOLD,
				verificationSimilarityThreshold);
			CheckError("InnoIdentification::SetParameters", errorCode);
			break;
		default:
			break;
		}		

		errorCode = IEngine_SetParameter(CFG_IDENTIFICATION_SPEED,
			identificationSpeed);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_IFACE_DETECT_FORCED,
			detectForced);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_BEST_CANDIDATES_COUNT,
			bestCandidates);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_IFACE_IGNORE_MULTIPLE_FACES,
			multipleFace);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_IFACE_DETECTION_MODE,
			detected);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_IFACE_EXTRACTION_MODE,
			extraction);
		CheckError("InnoIdentification::SetParameters", errorCode);

		errorCode = IEngine_SetParameter(CFG_IFACE_DETECTION_THRESHOLD,
			detectionThreshold);
		CheckError("InnoIdentification::SetParameters", errorCode);
		
	}

	void InnoIdentification::LoadConnection(int option)
	{
		int errorCode = IENGINE_E_NOERROR;
		
		handleConnect = IEngine_InitConnection();

		if (handleConnect != NULL)
		{
			errorCode = IEngine_SelectConnection(handleConnect);
			CheckError("InnoIdentification::LoadConnection", errorCode);
			if (errorCode == IENGINE_E_NOERROR) {
				SetParameters(option);
				errorCode = IEngine_Connect(connectDatabase.c_str());
				CheckError("InnoIdentification::LoadConnection", errorCode);
				userEnroll = IEngine_InitUser();
			}
		}
		else {
			ASS_ERROR_PROFILE_SCOPE("InnoIdentification::LoadConnection", 
				"IEngine_InitConnection ERROR");			
		}
	}

	void InnoIdentification::CloseConnection()
	{
		int errorCode = IENGINE_E_NOERROR;
		errorCode = IEngine_FreeUser(userEnroll);
		CheckError("InnoIdentification::CloseConnection", errorCode);
		errorCode = IEngine_CloseConnection(handleConnect);
		CheckError("InnoIdentification::CloseConnection", errorCode);
	}

	void InnoIdentification::Enroll(const unsigned char* tpt,
		int size, int* id)
	{
		int errorCode = IENGINE_E_NOERROR;
		
		try
		{
			errorCode = IEngine_AddFaceTemplate(userEnroll, tpt, size);
			CheckError("InnoIdentification::Enroll", errorCode);
			if (errorCode == IENGINE_E_NOERROR) {

				errorCode = IEngine_SelectConnection(handleConnect);
				CheckError("InnoIdentification::Enroll", errorCode);

				if (errorCode == IENGINE_E_NOERROR) {

					errorCode = IEngine_RegisterUser(userEnroll, id);
					CheckError("InnoIdentification::Enroll", errorCode);

				}

			}
		}
		catch (const std::exception& ex)
		{
			ASS_ERROR_PROFILE_SCOPE("InnoIdentification::Enroll",
				ex.what());
		}
				
		ClearEnroll();
	}

	void InnoIdentification::AddTemplate(const unsigned char* tpt, int size)
	{
		int errorCode = IENGINE_E_NOERROR;
		errorCode = IEngine_SelectConnection(handleConnect);
		CheckError("InnoIdentification::AddTemplate", errorCode);
		if (errorCode == IENGINE_E_NOERROR) {
			try
			{
				errorCode = IEngine_AddFaceTemplate(userEnroll, tpt, size);
				CheckError("InnoIdentification::AddTemplate", errorCode);
			}
			catch (const std::exception& ex)
			{
				ASS_ERROR_PROFILE_SCOPE("InnoIdentification::AddTemplate",
					ex.what());
			}
			
		}
	}

	void InnoIdentification::AddTemplate(const unsigned char* tpt, int size, int id)
	{
		int errorCode = IENGINE_E_NOERROR;
		IENGINE_USER user = IEngine_InitUser();

		errorCode = IEngine_SelectConnection(handleConnect);
		CheckError("InnoIdentification::AddTemplate", errorCode);

		if (errorCode == IENGINE_E_NOERROR) {

			errorCode = IEngine_GetUser(user, id);
			CheckError("InnoIdentification::AddTemplate", errorCode);
			if (errorCode == IENGINE_E_NOERROR) {
				errorCode = IEngine_AddFaceTemplate(user, tpt, size);
				CheckError("InnoIdentification::AddTemplate", errorCode);
			}
		}
		errorCode = IEngine_ClearUser(user);
		CheckError("InnoIdentification::AddTemplate", errorCode);
		errorCode = IEngine_FreeUser(user);
		CheckError("InnoIdentification::AddTemplate", errorCode);
	}

	unsigned char* InnoIdentification::GetTemplate(int id, int* lenght, int index)
	{
		int errorCode = IENGINE_E_NOERROR;
		IENGINE_USER user = IEngine_InitUser();

		errorCode = IEngine_SelectConnection(handleConnect);
		CheckError("InnoIdentification::GetTemplate", errorCode);

		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_GetUser(user, id);
			CheckError("InnoIdentification::GetTemplate", errorCode);

			if (errorCode == IENGINE_E_NOERROR) {
				errorCode = IEngine_GetFaceTemplate(user, index, NULL, lenght);
				CheckError("InnoIdentification::GetTemplate", errorCode);
				if (errorCode == IENGINE_E_NOERROR) {
					unsigned char* templateReturn = new unsigned char[*lenght];
					errorCode = IEngine_GetFaceTemplate(user, index, templateReturn, lenght);
					CheckError("InnoIdentification::GetTemplate", errorCode);
					if (errorCode == IENGINE_E_NOERROR) {
						errorCode = IEngine_ClearUser(user);
						CheckError("InnoIdentification::GetTemplate", errorCode);
						errorCode = IEngine_FreeUser(user);
						CheckError("InnoIdentification::GetTemplate", errorCode);
						return templateReturn;
					}
				}

			}

		}
		errorCode = IEngine_ClearUser(user);
		CheckError("InnoIdentification::GetTemplate", errorCode);
		errorCode = IEngine_FreeUser(user);
		CheckError("InnoIdentification::GetTemplate", errorCode);

		return nullptr;
	}

	void InnoIdentification::EnrollWithData(const unsigned char* tpt, 
		int size, const unsigned char* data, int dataLength, int* id)
	{
		int errorCode = IENGINE_E_NOERROR;

		try
		{
			errorCode = IEngine_AddFaceTemplate(userEnroll, tpt, size);
			CheckError("InnoIdentification::EnrollWithData", errorCode);

			if (errorCode == IENGINE_E_NOERROR)
			{
				errorCode = IEngine_SetCustomData(userEnroll, data, dataLength);
				CheckError("InnoIdentification::EnrollWithData", errorCode);

				if (errorCode == IENGINE_E_NOERROR) {
					errorCode = IEngine_SelectConnection(handleConnect);
					CheckError("InnoIdentification::EnrollWithData", errorCode);

					if (errorCode == IENGINE_E_NOERROR) {
						errorCode = IEngine_RegisterUser(userEnroll, id);
					}
				}
			}

		}
		catch (const std::exception& ex)
		{
			ASS_ERROR_PROFILE_SCOPE("InnoIdentification::Enroll",
				ex.what());
		}

		ClearEnroll();
	}
	
	void InnoIdentification::ClearEnroll()
	{
		int errorCode = IENGINE_E_NOERROR;
		errorCode = IEngine_ClearUser(userEnroll);
		CheckError("InnoIdentification::ClearEnroll", errorCode);
	}

	void InnoIdentification::Remove(int id)
	{
		int errorCode = IENGINE_E_NOERROR;

		errorCode = IEngine_SelectConnection(handleConnect);
		CheckError("InnoIdentification::Remove", errorCode);

		if (errorCode == IENGINE_E_NOERROR) {
			errorCode = IEngine_RemoveUser(id);
			CheckError("InnoIdentification::Remove", errorCode);
			if (errorCode == IENGINE_E_NOERROR)
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%04d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
					ASSInterface::DateTime::Now().c_str(), indexChannel, id, "User remove.");
			}
		}
	}

	void InnoIdentification::Find(const unsigned char* tpt, int size, int* id, int* score)
	{
		int errorCode = IENGINE_E_NOERROR;
		
		IENGINE_USER user = IEngine_InitUser();

		try
		{
			errorCode = IEngine_AddFaceTemplate(user, tpt, size);
			CheckError("InnoIdentification::Find", errorCode);
			if (errorCode == IENGINE_E_NOERROR) {
				errorCode = IEngine_SelectConnection(handleConnect);
				CheckError("InnoIdentification::Find", errorCode);
				if (errorCode == IENGINE_E_NOERROR) {
					errorCode = IEngine_GetParameter(CFG_BEST_CANDIDATES_COUNT, &bestCandidates);
					
					errorCode = IEngine_FindUser(user, id, score);					
					CheckError("InnoIdentification::Find", errorCode);													
				}
					

			}
		}
		catch (const std::exception& ex)
		{
			ASS_ERROR_PROFILE_SCOPE("InnoIdentification::Find",
				ex.what());
		}
					
		
		errorCode = IEngine_ClearUser(user);
		CheckError("InnoIdentification::Find", errorCode);
		errorCode = IEngine_FreeUser(user);
		CheckError("InnoIdentification::Find", errorCode);
	}

	void InnoIdentification::Match(const unsigned char* tpt1, int size1, 
		const unsigned char* tpt2, int size2, int* score)
	{
		int errorCode = IENGINE_E_NOERROR;
		IENGINE_USER user1 = IEngine_InitUser();
		IENGINE_USER user2 = IEngine_InitUser();

		errorCode = IEngine_AddFaceTemplate(user1, tpt1, size1);
		CheckError("InnoIdentification::Match", errorCode);

		if (errorCode == IENGINE_E_NOERROR) {

			errorCode = IEngine_AddFaceTemplate(user2, tpt2, size2);
			CheckError("InnoIdentification::Match", errorCode);

			if (errorCode == IENGINE_E_NOERROR) {

				errorCode = IEngine_SelectConnection(handleConnect);
				CheckError("InnoIdentification::Match", errorCode);

				if (errorCode == IENGINE_E_NOERROR) {
					errorCode = IEngine_MatchUsersModalities(user1, user2,
						IENGINE_MATCHING_MODALITY::MODALITY_FACE, score);
					CheckError("InnoIdentification::Match", errorCode);
				}
			}
		}
		errorCode = IEngine_ClearUser(user1);
		CheckError("InnoIdentification::Match", errorCode);
		errorCode = IEngine_FreeUser(user1);
		CheckError("InnoIdentification::Match", errorCode);
		errorCode = IEngine_ClearUser(user2);
		CheckError("InnoIdentification::Match", errorCode);
		errorCode = IEngine_FreeUser(user2);
		CheckError("InnoIdentification::Match", errorCode);

	}

	void InnoIdentification::GetParamsDetectFace()
	{
		std::string nameFile = "face" + std::to_string(indexChannel) + ".txt";
		configurationFace = ASSInterface::Configuration::CreateConfigFace();
		configurationFace->SetPath(nameFile);
		configurationFace->ParseToObject();
		
		std::any anyDetection = configurationFace->GetParam("CFG_IFACE_DETECTION_MODE");
		std::any anyExtraction = configurationFace->GetParam("CFG_IFACE_EXTRACTION_MODE");
		std::any anyForced = configurationFace->GetParam("CFG_IFACE_DETECT_FORCED");
		std::any anyMultipleFaces = configurationFace->GetParam("CFG_IFACE_IGNORE_MULTIPLE_FACES");
		
		detected = std::any_cast<int>(anyDetection);
		extraction = std::any_cast<int>(anyExtraction);
		detectForced = std::any_cast<int>(anyForced);
		multipleFace = std::any_cast<int>(anyMultipleFaces);
	}

	void InnoIdentification::GetParamsEnroll()
	{
		std::string nameFile = "enroll" + std::to_string(indexChannel) + ".txt";
		configurationEnroll = ASSInterface::Configuration::CreateConfigEnroll();
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
		deduplicationSimilarityThreshold = std::any_cast<int>(anySimilarityThreshold);
		gallery = std::any_cast<int>(anyGallery);
		deduplicate = std::any_cast<int>(anyDeduplicate);
		concatenate = std::any_cast<int>(anyConcatenate);
		maxTemplates = std::any_cast<int>(anyMaxTemplates);
		concatenateMode = std::any_cast<int>(anyConcatenateMode);
		minScore = std::any_cast<int>(anyMinScore);
		maxScore = std::any_cast<int>(anyMaxScore);		
	}

	void InnoIdentification::GetParamsControlEntry()
	{
		std::string nameFile = "entry" + std::to_string(indexChannel) + ".txt";
		configurationEntry = ASSInterface::Configuration::CreateConfigEntry();
		configurationEntry->SetPath(nameFile);
		configurationEntry->ParseToObject();

		std::any anyIdenSimilarity = configurationEntry->GetParam("CFG_IDENTIFICATION_SIMILARITY_THRESHOLD");
		std::any anyVeriSimilarity = configurationEntry->GetParam("CFG_VERIFICATION_SIMILARITY_THRESHOLD");
		std::any anyBestCandidates = configurationEntry->GetParam("CFG_BEST_CANDIDATES_COUNT");
		std::any anyIdentificationSpeed = configurationEntry->GetParam("CFG_IDENTIFICATION_SPEED");

		identificationSimilarityThreshold = std::any_cast<int>(anyIdenSimilarity);
		verificationSimilarityThreshold = std::any_cast<int>(anyVeriSimilarity);
		bestCandidates = std::any_cast<int>(anyBestCandidates);
		identificationSpeed = std::any_cast<int>(anyIdentificationSpeed);
	}

	void InnoIdentification::CheckError(std::string function, int errorCode)
	{
		if (errorCode != IENGINE_E_NOERROR) {
			ASS_ERROR_PROFILE_SCOPE(function.c_str(), errorIDkit->GetError(errorCode).c_str());
		}
	}

}

