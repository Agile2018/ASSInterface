#include "hzpch.h"
#include "InnoTask.h"

namespace ASSInterface {
	InnoTask::InnoTask(int channel)
	{
		indexChannel = channel;
		innoIdentify = ASSInterface::Identification::CreateInnoIdentify(channel);
		GetParamsEnroll();		
		base64 = ASSInterface::Base64::Create();
	}
	InnoTask::~InnoTask()
	{
	}
	void InnoTask::CloseConnection()
	{
		innoIdentify->CloseConnection();
		isConnect = false;
	}
	void InnoTask::ResetParameters()
	{		
		GetParamsEnroll();
		innoIdentify->ResetParameters();
		isConnect = false;
	}
	void InnoTask::DoTask(DetectSpecification specDetect)
	{
		if (&specDetect.templateData[0] != NULL)
		{
			switch (specDetect.task)
			{
			case ENROLL_PERSON:	
				BuildForEnrollment(specDetect);
				break;			
			case CONTROL_ENTRY:				
				ExecuteControlEntry(specDetect);				
				break;
			case CONTROL_ENTRY_TRACK0:
			case CONTROL_ENTRY_TRACK1:
			case CONTROL_ENTRY_TRACK2:
				ExecuteControlEntry(specDetect);				
				break;		
			case EDIT_PERSON:
				BifurcateTaskTemplate(specDetect);
				break;
			default:
				break;
			}

		}
	}
	void InnoTask::DoTask(concurrent_vector<DetectSpecification> concurrentDetection, 
		std::string gallery)
	{
		if (concurrentDetection.size() > 0) {
			
			Import(concurrentDetection, gallery);
				
		}
	}
	void InnoTask::DoTask(std::vector<DetectSpecification> specDetects)
	{

		switch (specDetects[0].task)
		{
		case ENROLL_WITH_TEMPLATES:
			EnrollPersonAndTemplates(specDetects);
			break;
		case ADD_TEMPLATES_OF_FILES:
			AddTemplatesToEnrolled(specDetects);
			break;
		case SET_TEMPLATE_OF_FILES:
			SetFaceTemplate(specDetects);
			break;
		default:
			break;
		}

		
	}
	void InnoTask::Enroll(TemplateSpecification specTemplate, int* id)
	{
		
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&specTemplate.templateData[0]);
		const unsigned char* customData = reinterpret_cast<const unsigned char*>(
			&specTemplate.faceSerialized[0]);
		innoIdentify->SetCustomData(customData, (int)specTemplate.faceSerialized.size());
		innoIdentify->Enroll(templateData, (int)specTemplate.templateData.size(), id);

	}
	void InnoTask::EnrollTemplates(std::vector<TemplateSpecification> specTemplate, int* id)
	{
		int countTemplates = 0, index = -1, quality = 0;

		for (int i = 0; i < specTemplate.size(); i++)
		{
			if (specTemplate[i].quality > quality)
			{
				quality = specTemplate[i].quality;
				index = i;
			}
		}

		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specTemplate[index].templateData[0]);

		if (EnrollmentIsExecuted(templateData, specTemplate[index].size)) {
			if (concatenate == 1)
			{
				for (int i = 0; i < specTemplate.size(); i++)
				{
					if (i != index)
					{
						const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
							&specTemplate[i].templateData[0]);

						if (concatenateMode == 0)
						{
							int score = -1;
							innoIdentify->Match(templateData, specTemplate[0].size,
								tptData, specTemplate[i].size, &score);
							if (score <= maxScore && score >= minScore &&
								countTemplates <= maxTemplates) {

								innoIdentify->AddTemplate(tptData, specTemplate[i].size);
								countTemplates++;
							}
							else
							{
								ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
									ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
							}
						}
						else if (countTemplates <= maxTemplates)
						{
							innoIdentify->AddTemplate(tptData, specTemplate[i].size);
							countTemplates++;
						}
					}
					
				}
			}

			if (!specTemplate[index].faceSerialized.empty())
			{
				const unsigned char* customData = reinterpret_cast<const unsigned char*>(
					&specTemplate[index].faceSerialized[0]);
				innoIdentify->SetCustomData(customData, (int)specTemplate[index].faceSerialized.size());
			}

			innoIdentify->Enroll(templateData, (int)specTemplate[index].templateData.size(), id);
			if (*id != 0)
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], templates: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), indexChannel, *id, countTemplates, "Enrolled");
			}
		}
		
	}
	void InnoTask::Remove(int id)
	{
		innoIdentify->Remove(id);
	}
	void InnoTask::Import(concurrent_vector<DetectSpecification> concurrentDetection, 
		std::string gallery)
	{				
		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}
		
		int initCount = (int)concurrentDetection.size();

		parallel_for(blocked_range<size_t>(0, concurrentDetection.size()),
			[=](const blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i != r.end(); ++i) {
					EnrollImport(concurrentDetection[i], gallery);
				}
			}
		);

		concurrentDetection.clear();		
		shootEnd.on_next(countEnrolled);

		int resultRejected = initCount - countEnrolled;

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], enrolled: [%03d], rejected: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), 
			indexChannel, countEnrolled, resultRejected, "Enrolled");
		
		countEnrolled = 0;
		
	}
	int InnoTask::GetFaceCount(int id)
	{
		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}

		return innoIdentify->GetFaceCount(id);
	}

	std::vector<int> InnoTask::GetFaceQuality(int id, int index)
	{
		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_VERIFICATION);
			isConnect = true;
		}

		int quality = 0, lenghtMain = 0, lenghtTemp = 0, score = 0;
		std::vector<int> result;

		innoIdentify->GetFaceQuality(id, index, &quality);

		unsigned char* templateMain = innoIdentify->GetTemplate(id, &lenghtMain, 0);
		unsigned char* templateTemp = innoIdentify->GetTemplate(id, &lenghtTemp, index);

		innoIdentify->Match(templateMain, lenghtMain,
			templateTemp, lenghtTemp, &score);

		result.push_back(quality);
		result.push_back(score);
		
		templateMain = NULL;
		templateTemp = NULL;

		return result;
	}
	void InnoTask::RemoveFace(int id, int index)
	{
		innoIdentify->RemoveFace(id, index);
	}
	void InnoTask::GetParamsEnroll()
	{
		std::string nameFile = "enroll" + std::to_string(indexChannel) + ".txt";
		configurationEnroll = ASSInterface::Configuration::CreateConfigEnroll();
		configurationEnroll->SetPath(nameFile);
		configurationEnroll->ParseToObject();
		
		std::any anyGallery = configurationEnroll->GetParam("AFACE_GALLERY");
		std::any anyDeduplicate = configurationEnroll->GetParam("AFACE_DEDUPLICATE");
		std::any anyConcatenate = configurationEnroll->GetParam("AFACE_CONCATENATE");	
		std::any anyMaxTemplates = configurationEnroll->GetParam("AFACE_MAX_TEMPLATES");
		std::any anyConcatenateMode = configurationEnroll->GetParam("AFACE_CONCATENATE_MODE");	
		std::any anyMinScore = configurationEnroll->GetParam("AFACE_MIN_SCORE");
		std::any anyMaxScore = configurationEnroll->GetParam("AFACE_MAX_SCORE");

		gallery = std::any_cast<int>(anyGallery);
		deduplicate = std::any_cast<int>(anyDeduplicate);
		concatenate = std::any_cast<int>(anyConcatenate);	
		maxTemplates = std::any_cast<int>(anyMaxTemplates);
		concatenateMode = std::any_cast<int>(anyConcatenateMode);
		minScore = std::any_cast<int>(anyMinScore);
		maxScore = std::any_cast<int>(anyMaxScore);
	}
	void InnoTask::ExecuteControlEntry(DetectSpecification specDetect)
	{		
		int userID[BEST_CANDIDATES];
		int score[BEST_CANDIDATES];
		
		if (!isConnect)
		{			
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}		

		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&specDetect.templateData[0]);
		innoIdentify->Find(templateData, (int)specDetect.templateData.size(), userID, score);

		if (userID[0] == 0)
		{
			if (CheckUnidentified(templateData, (int)specDetect.templateData.size()))
			{			
				SendUnidentified(specDetect);
			}
		}
		else
		{
			SendIdentified(specDetect, userID[0], score[0]);
		}

		templateData = NULL;
		
	}
	
	bool InnoTask::CheckUnidentified(const unsigned char* data, int size)
	{
		int score = -1;

		if (!unidentified.empty())
		{
			const unsigned char* templatePtrGuide = reinterpret_cast<const unsigned char*>(&unidentified[0]);

			innoIdentify->Match(templatePtrGuide,
				(int)unidentified.size(), data, size, &score);
			
			if (score == 0)
			{
				unidentified.clear();
				unidentified.assign(data, data + size);
				return true;
			}
			else {
				return false;
			}
		}
		else {
			unidentified.assign(data, data + size);
		}
		return true;
	}
	void InnoTask::SendUnidentified(DetectSpecification& specDetect)
	{
		IdentitySpecification specIdentify;

		specIdentify.id = 0;
		specIdentify.match = specDetect.quality;
		specIdentify.channel = indexChannel;
		specIdentify.size = specDetect.sizeTemplate;		
		specIdentify.cropLength = specDetect.cropSpec.length;		
		specIdentify.cropWidth = specDetect.cropSpec.width;
		specIdentify.cropHeight = specDetect.cropSpec.height;
		specIdentify.cropData.assign(&specDetect.cropSpec.cropData[0],
			&specDetect.cropSpec.cropData[0] + specDetect.cropSpec.cropData.size());
		specIdentify.templateData.assign(&specDetect.templateData[0],
			&specDetect.templateData[0] + specDetect.templateData.size());
		specIdentify.task = specDetect.task;
		specIdentify.medianICAO = specDetect.medianICAO;

		if (!specDetect.faceSerialized.empty())
		{
			specIdentify.faceSerialized.assign(&specDetect.faceSerialized[0],
				&specDetect.faceSerialized[0] + specDetect.faceSerialized.size());
		}

		shootSpecIdentify.on_next(specIdentify);

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], quality: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), 
			ASSInterface::DateTime::Now().c_str(), indexChannel, 
			specDetect.quality, "Unidentified");
		
	}
	void InnoTask::SendIdentified(DetectSpecification& specDetect, 
		int id, int score)
	{
				
		IdentitySpecification specIdentify;

		dbMongo->Get(std::to_string(id).c_str());

		specIdentify.id = id;
		specIdentify.channel = indexChannel;
		specIdentify.match = score;
		specIdentify.name = dbMongo->GetEntitySpecification().name;
		specIdentify.lastName = dbMongo->GetEntitySpecification().lastName;
		specIdentify.document = dbMongo->GetEntitySpecification().identification;
		specIdentify.imageDB = dbMongo->GetEntitySpecification().dataImage;
		specIdentify.docObverse = dbMongo->GetEntitySpecification().dataDocObverse;
		specIdentify.docReverse = dbMongo->GetEntitySpecification().dataDocReverse;
		specIdentify.gallery = dbMongo->GetEntitySpecification().type;		
		specIdentify.gallery1 = dbMongo->GetEntitySpecification().type1;
		specIdentify.cropLength = specDetect.cropSpec.length;		
		specIdentify.cropWidth = specDetect.cropSpec.width;
		specIdentify.cropHeight = specDetect.cropSpec.height;
		specIdentify.cropData.assign(&specDetect.cropSpec.cropData[0],
			&specDetect.cropSpec.cropData[0] + specDetect.cropSpec.cropData.size());
		specIdentify.task = specDetect.task;

		shootSpecIdentify.on_next(specIdentify);
		
		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), 
			ASSInterface::DateTime::Now().c_str(), indexChannel, id, score, "Identified");
	}

	void InnoTask::AddTemplatesToEnrolled(std::vector<DetectSpecification> specDetects) {
		int countTemplates = 0, lenght = 0;
		std::vector<ASSInterface::EntityImage> images;

		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}

		unsigned char* tptZero = innoIdentify->GetTemplate(specDetects[0].id, &lenght, 0);		
		countTemplates = innoIdentify->GetFaceCount(specDetects[0].id);

		if (tptZero != NULL && countTemplates < maxTemplates) {
			countTemplates = 0;
			const unsigned char* tptMain = reinterpret_cast<const unsigned char*>(tptZero);
			for (int i = 0; i < specDetects.size(); i++) {
				const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
					&specDetects[i].templateData[0]);

				if (concatenate == 1) {
					int count = 0;
					if (concatenateMode == 0) {
						int score = -1;
						innoIdentify->Match(tptMain, lenght, tptData, specDetects[i].sizeTemplate, &score);
						if (score <= maxScore && score >= minScore) {

							innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate, specDetects[i].id);
							count = innoIdentify->GetFaceCount(specDetects[i].id);							
							countTemplates++;
						}
						else
						{
							ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
								ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
						}
					}
					else {
						innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate, specDetects[i].id);
						count = innoIdentify->GetFaceCount(specDetects[i].id);						
						countTemplates++;
					}

					if (count != 0)
					{
						AddImage(specDetects[i].cropSpec.cropData, images, count - 1, specDetects[i].id);
					}

					if (count == maxTemplates) break;
				}
				else
				{
					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_concatenate: [%01d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
						ASSInterface::DateTime::Now().c_str(), indexChannel, concatenate, "Cannot be added templates.");
					break;
				}

				tptData = NULL;
			}

			if (!images.empty())
			{

				for (size_t i = 0; i < images.size(); i++)
				{
					dbMongo->Add(images[i]);
				}
				
				shootEnd.on_next(countTemplates);
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], templates: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0),
					ASSInterface::DateTime::Now().c_str(), indexChannel, countTemplates, "Add templates");
				images.clear();
			}

		}
		else
		{
			ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], templates: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
				ASSInterface::DateTime::Now().c_str(), indexChannel, countTemplates, "Cannot be added templates.");
		}
		
		tptZero = NULL;

	}
	void InnoTask::EnrollPersonAndTemplates(std::vector<DetectSpecification> specDetects)
	{
		int countTemplates = 0;
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specDetects[0].templateData[0]);

		if (EnrollmentIsExecuted(templateData, specDetects[0].sizeTemplate))
		{

			if (concatenate == 1)
			{
				SendUnidentified(specDetects[0]);

				for (int i = 1; i < specDetects.size(); i++)
				{
					const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
						&specDetects[i].templateData[0]);

					if (concatenateMode == 0)
					{
						int score = -1;
						innoIdentify->Match(templateData, specDetects[0].sizeTemplate,
							tptData, specDetects[i].sizeTemplate, &score);
						if (score <= maxScore && score >= minScore && 
							countTemplates <= maxTemplates) {

							SendUnidentified(specDetects[i]);
							//innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate);
							countTemplates++;
						}
						else
						{
							ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
								ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
						}
					}
					else if(countTemplates <= maxTemplates)
					{
						//innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate);
						SendUnidentified(specDetects[i]);
						countTemplates++;
					}
				}

				/*if (!specDetects[0].faceSerialized.empty())
				{
					const unsigned char* customData = reinterpret_cast<const unsigned char*>(
						&specDetects[0].faceSerialized[0]);
					innoIdentify->SetCustomData(customData, (int)specDetects[0].faceSerialized.size());
				}

				int id = -1;
				innoIdentify->Enroll(templateData, specDetects[0].sizeTemplate, &id);

				if (id > 0)
				{
					SendEnrolled(specDetects[0], id, countTemplates);
				}*/
			}
			else
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_concatenate: [%01d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
					ASSInterface::DateTime::Now().c_str(), indexChannel, concatenate, "Cannot be added templates.");
			}
			
		}

		templateData = NULL;
	}

	bool InnoTask::EnrollmentIsExecuted(const unsigned char* data, int size)
	{		
		int userID[BEST_CANDIDATES];
		int score[BEST_CANDIDATES];

		if (!isConnect)
		{
			if (deduplicate == 0)
				innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			if (deduplicate == 1)
				innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_DEDUPLICATION);
			isConnect = true;
		}

		if (deduplicate == 0) return true;

		innoIdentify->Find(data, size, userID, score);

		if (userID[0] == 0)
		{
			return true;
		}
		else
		{
			ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1), ASSInterface::DateTime::Now().c_str(), indexChannel, 
				userID[0], score, "The person is already registered");
		}
		return false;
	}

	void InnoTask::SendEnrolled(DetectSpecification& specDetect, int id, int numberTemplates)
	{
		IdentitySpecification specIdentify;

		specIdentify.id = id;
		specIdentify.match = specDetect.quality;
		specIdentify.channel = indexChannel;
		specIdentify.size = specDetect.sizeTemplate;
		specIdentify.cropLength = specDetect.cropSpec.length;
		specIdentify.cropWidth = specDetect.cropSpec.width;
		specIdentify.cropHeight = specDetect.cropSpec.height;
		specIdentify.cropData.assign(&specDetect.cropSpec.cropData[0],
			&specDetect.cropSpec.cropData[0] + specDetect.cropSpec.cropData.size());
		specIdentify.templateData.assign(&specDetect.templateData[0],
			&specDetect.templateData[0] + specDetect.templateData.size());
		specIdentify.task = specDetect.task;

		shootSpecIdentify.on_next(specIdentify);

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], quality: [%03d], templates: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), indexChannel, id, specDetect.quality, numberTemplates, "Enrolled");
	}

	void InnoTask::BuildForEnrollment(DetectSpecification specDetect)
	{
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specDetect.templateData[0]);
		if (EnrollmentIsExecuted(templateData, specDetect.sizeTemplate)) {
			
			SendUnidentified(specDetect);
		}
		templateData = nullptr;
	}	

	void InnoTask::EnrollImport(DetectSpecification specDetect, std::string gallery)
	{		
		int userID[BEST_CANDIDATES];
		int score[BEST_CANDIDATES];
		bool flagEnroll = true;

		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specDetect.templateData[0]);
		int sizeTemp = specDetect.sizeTemplate;
		std::vector<unsigned char> photoGallery(&specDetect.cropSpec.cropData[0], 
			&specDetect.cropSpec.cropData[0] + specDetect.cropSpec.length);
		const unsigned char* customData = reinterpret_cast<const unsigned char*>(
			&specDetect.faceSerialized[0]);
		int id = -1;

		if (deduplicate == 1) {
			
			innoIdentify->Find(templateData, sizeTemp, userID, score);

			if (userID[0] != 0)
			{				
				flagEnroll = false;
				/*ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%04d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
					ASSInterface::DateTime::Now().c_str(), indexChannel, userID[0],
					"The person is already registered.");*/
												
			}
			
		}
		
		if (flagEnroll)
		{
			innoIdentify->EnrollImport(templateData, sizeTemp, customData,
				(int)specDetect.faceSerialized.size(), &id);

			if (id > 0)
			{
				EnrollImportDatabase(photoGallery, id, gallery);
				countEnrolled++;
			}
		}

		customData = NULL;
		templateData = NULL;
		photoGallery.clear();
	}

	void InnoTask::AddImage(std::vector<unsigned char> cropImage, 
		std::vector<ASSInterface::EntityImage>& images, int index, int id)
	{
		ASSInterface::EntityImage ent;

		ent.id = std::to_string(id);		
		std::string data = base64->base64_encode(&cropImage[0],
			(int)cropImage.size());
		ent.data = data;
		ent.index = index;
		images.push_back(ent);

	}

	void InnoTask::AddImage(std::vector<unsigned char> cropImage, int index, int id)
	{
		ASSInterface::EntityImage ent;

		ent.id = std::to_string(id);
		std::string data = base64->base64_encode(&cropImage[0],
			(int)cropImage.size());
		ent.data = data;
		ent.index = index;
		dbMongo->Add(ent);

	}

	void InnoTask::AddFaceTemplate(DetectSpecification specDetect) {
		int lenght = 0;

		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}

		unsigned char* tptZero = innoIdentify->GetTemplate(idFace, &lenght, 0);
		if (tptZero != NULL) {
			const unsigned char* tptMain = reinterpret_cast<const unsigned char*>(tptZero);
			const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
				&specDetect.templateData[0]);

			if (concatenate == 1) {
				int count = 0;

				if (concatenateMode == 0) {
					int score = -1;
					innoIdentify->Match(tptMain, lenght, tptData, specDetect.sizeTemplate, &score);
					if (score <= maxScore && score >= minScore) {

						innoIdentify->AddTemplate(tptData, specDetect.sizeTemplate, idFace);
						count = innoIdentify->GetFaceCount(idFace);
					}
					else
					{
						ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
							ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
					}
				}
				else {
					innoIdentify->AddTemplate(tptData, specDetect.sizeTemplate, idFace);
					count = innoIdentify->GetFaceCount(idFace);
				}

				if (count != 0)
				{
					AddImage(specDetect.cropSpec.cropData, count - 1, idFace);
					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], templates: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), indexChannel, idFace, count, "Add Template.");
					shootEnd.on_next(count);
					
				}
			}
			else
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_concatenate: [%01d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
					ASSInterface::DateTime::Now().c_str(), indexChannel, concatenate, "Cannot be added templates.");
				
			}

			tptData = NULL;
			tptMain = NULL;
		}

		tptZero = NULL;
	}
	void InnoTask::BifurcateTaskTemplate(DetectSpecification specDetect) {
		if (indexTemplate != -1)
		{
			UpdateFaceTemplate(specDetect);
		}
		else
		{
			AddFaceTemplate(specDetect);
		}
	}

	void InnoTask::UpdateFaceTemplate(DetectSpecification specDetect) {
		int lenght = 0, count = 0;

		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}

		unsigned char* tptZero = innoIdentify->GetTemplate(idFace, &lenght, 0);

		if (tptZero != NULL) {
			const unsigned char* tptMain = reinterpret_cast<const unsigned char*>(tptZero);
			const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
				&specDetect.templateData[0]);

			if (concatenate == 1) {

				if (concatenateMode == 0) {
					int score = -1;
					innoIdentify->Match(tptMain, lenght, tptData, specDetect.sizeTemplate, &score);
					if (score <= maxScore && score >= minScore) {

						innoIdentify->SetFaceTemplate(idFace, indexTemplate,
							tptData, specDetect.sizeTemplate);
						count++;
					}
					else
					{
						ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
							ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
					}
				}
				else {
					innoIdentify->SetFaceTemplate(idFace, indexTemplate,
						tptData, specDetect.sizeTemplate);
					count++;
				}

				if (count != 0)
				{
					SetTemplateDatabase(specDetect.cropSpec.cropData,
						idFace, indexTemplate);
					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], id: [%05d], index: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0), ASSInterface::DateTime::Now().c_str(), indexChannel, idFace, indexTemplate, "Change Template.");
					shootEnd.on_next(count);
					
				}

			}
			else
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_concatenate: [%01d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
					ASSInterface::DateTime::Now().c_str(), indexChannel, concatenate, "Cannot be added templates.");
				
			}

			tptData = NULL;
			tptMain = NULL;
		}

		tptZero = NULL;
	}

	void InnoTask::SetFaceTemplate(std::vector<DetectSpecification> specDetects)
	{
		int lenght = 0, count = 0;

		if (!isConnect)
		{
			innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			isConnect = true;
		}

		unsigned char* tptZero = innoIdentify->GetTemplate(specDetects[0].id, &lenght, 0);

		if (tptZero != NULL) {
			
			const unsigned char* tptMain = reinterpret_cast<const unsigned char*>(tptZero);

			for (int i = 0; i < specDetects.size(); i++) {
				const unsigned char* tptData = reinterpret_cast<const unsigned char*>(
					&specDetects[i].templateData[0]);

				if (concatenate == 1) {

					if (concatenateMode == 0) {
						int score = -1;
						innoIdentify->Match(tptMain, lenght, tptData, specDetects[i].sizeTemplate, &score);
						if (score <= maxScore && score >= minScore) {

							innoIdentify->SetFaceTemplate(specDetects[i].id, specDetects[i].indexFace,
								tptData, specDetects[i].sizeTemplate);							
							count++;
						}
						else
						{
							ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_score: [%03d], prm_max_score: [%03d], score: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
								ASSInterface::DateTime::Now().c_str(), indexChannel, minScore, maxScore, score, "Rejected");
						}
					}
					else {
						innoIdentify->SetFaceTemplate(specDetects[i].id, specDetects[i].indexFace,
							tptData, specDetects[i].sizeTemplate);
						count++;
					}

					if (count != 0)
					{
						SetTemplateDatabase(specDetects[i].cropSpec.cropData, 
							specDetects[i].id, specDetects[i].indexFace);
					}

					
				}
				else
				{
					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_concatenate: [%01d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(2),
						ASSInterface::DateTime::Now().c_str(), indexChannel, concatenate, "Cannot be added templates.");
					break;
				}

				tptData = NULL;
			}
			

		}
		
		tptZero = NULL;

	}

	void InnoTask::SetTemplateDatabase(std::vector<unsigned char> cropImage, int id, int index)
	{
		ASSInterface::EntityImage ent;
		ent.id = std::to_string(id);
		ent.index = index;
		ent.data = base64->base64_encode(&cropImage[0],
			(int)cropImage.size());

		dbMongo->Update(ent);
	}

	void InnoTask::EnrollImportDatabase(std::vector<unsigned char> photoGallery, int id, std::string gallery)
	{
		ASSInterface::EntitySpecification entSpec;
		entSpec.id = std::to_string(id);
		entSpec.date = ASSInterface::DateTime::NowToLong();		
		entSpec.dataImage = base64->base64_encode(&photoGallery[0],
			(int)photoGallery.size());
		entSpec.type = gallery;
		dbMongo->Add(entSpec);

		ASSInterface::EntityImage entImage;
		entImage.id = std::to_string(id);
		entImage.data = entSpec.dataImage;
		entImage.index = 0;
		dbMongo->Add(entImage);

		ASSInterface::EntityEvent entEvent;
		entEvent.id = std::to_string(id);
		entEvent.date = ASSInterface::DateTime::NowToLong();
		entEvent.type = ASSInterface::EventAppType::enrollment;
		entEvent.description = "Enroll Person.";

		dbMongo->Add(entEvent);
	
	}
	
}