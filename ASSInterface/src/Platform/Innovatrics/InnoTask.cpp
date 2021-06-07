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
				ExecuteControlEntryTrack(specDetect);
				break;
			default:
				break;
			}

		}
	}
	void InnoTask::DoTask(concurrent_vector<DetectSpecification> concurrentDetection)
	{
		if (concurrentDetection.size() > 0) {
			
			Import(concurrentDetection);
				
		}
	}
	void InnoTask::DoTask(std::vector<DetectSpecification> specDetects)
	{

		switch (specDetects[0].task)
		{
		case ENROLL_WITH_TEMPLATES:
			EnrollPersonAndTemplates(specDetects);
			break;
		default:
			break;
		}

		
	}
	void InnoTask::Enroll(std::vector<char> templateFace, int* id)
	{
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(&templateFace[0]);
		innoIdentify->Enroll(templateData, templateFace.size(), id);

	}
	void InnoTask::EnrollTemplates(std::vector<TemplateSpecification> specTemplate, int* id)
	{
		int countTemplates = 0;
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specTemplate[0].templateData[0]);
		if (EnrollmentIsExecuted(templateData, specTemplate[0].size)) {
			if (concatenate == 1)
			{
				for (int i = 1; i < specTemplate.size(); i++)
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

			innoIdentify->Enroll(templateData, specTemplate[0].templateData.size(), id);
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
	void InnoTask::Import(concurrent_vector<DetectSpecification> concurrentDetection)
	{				
		if (!isConnect)
		{
			if (deduplicate == 0)
				innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_IDENTIFICATION);
			if (deduplicate == 1)
				innoIdentify->LoadConnection(SIMILARITY_THRESHOLD_DEDUPLICATION);
			isConnect = true;
		}
		
		parallel_for(blocked_range<size_t>(0, concurrentDetection.size()),
			[=](const blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i != r.end(); ++i) {
					EnrollImport(concurrentDetection[i]);
				}
			}
		);

		concurrentDetection.clear();		
		shootEnd.on_next(countEnrolled);

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], enrolled: [%05d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0),
			ASSInterface::DateTime::Now().c_str(), indexChannel, countEnrolled, "Enrolled");
		
		countEnrolled = 0;
		
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
	void InnoTask::ExecuteControlEntryTrack(DetectSpecification specDetect)
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

		if (userID[0] != 0)
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

	void InnoTask::EnrollPersonAndTemplates(std::vector<DetectSpecification> specDetects)
	{
		int countTemplates = 0;
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specDetects[0].templateData[0]);

		if (EnrollmentIsExecuted(templateData, specDetects[0].sizeTemplate))
		{
			if (concatenate == 1)
			{
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

							innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate);
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
						innoIdentify->AddTemplate(tptData, specDetects[i].sizeTemplate);
						countTemplates++;
					}
				}
			}

			int id = -1;
			innoIdentify->Enroll(templateData, specDetects[0].sizeTemplate, &id);

			if (id > 0)
			{
				SendEnrolled(specDetects[0], id, countTemplates);
			}
		}
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

	void InnoTask::EnrollImport(DetectSpecification specDetect)
	{		
		int userID[BEST_CANDIDATES];
		int score[BEST_CANDIDATES];
		
		const unsigned char* templateData = reinterpret_cast<const unsigned char*>(
			&specDetect.templateData[0]);

		if (deduplicate == 1) {
			
			innoIdentify->Find(templateData, specDetect.sizeTemplate, userID, score);

			if (userID[0] == 0)
			{
				int id = -1;
				innoIdentify->Enroll(templateData, specDetect.sizeTemplate, &id);
				if (id > 0)
				{
					EnrollImportDatabase(specDetect, id);
					countEnrolled++;
				}
			}
		}
		else
		{
			int id = -1;
			innoIdentify->Enroll(templateData, specDetect.sizeTemplate, &id);
			if (id > 0)
			{
				EnrollImportDatabase(specDetect, id);
				countEnrolled++;
			}
		}

		templateData = NULL;
		
	}

	void InnoTask::EnrollImportDatabase(DetectSpecification specDetect, int id)
	{
		ASSInterface::EntitySpecification entSpec;
		entSpec.id = std::to_string(id);
		entSpec.date = ASSInterface::DateTime::NowToLong();		
		entSpec.dataImage = base64->base64_encode(&specDetect.cropSpec.cropData[0], 
			specDetect.cropSpec.length);
		dbMongo->Add(entSpec);

		ASSInterface::EntityEvent entEvent;
		entEvent.id = std::to_string(id);
		entEvent.date = ASSInterface::DateTime::NowToLong();
		entEvent.type = ASSInterface::EventAppType::enrollment;
		entEvent.description = "Enroll Person.";

		dbMongo->Add(entEvent);

	}
	
}