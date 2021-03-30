#include "hzpch.h"
#include "JSONConfigEnroll.h"

namespace ASSInterface {
	JSONConfigEnroll::JSONConfigEnroll()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsEnroll[CFG_IFACE_DETECTION_THRESHOLD] = detectionThreshold;
		paramsEnroll[CFG_SIMILARITY_THRESHOLD] = similarityThreshold;
		paramsEnroll[AFACE_GALLERY] = gallery;
		paramsEnroll[AFACE_DEDUPLICATE] = deduplicate;
		paramsEnroll[AFACE_CONCATENATE] = concatenate;
		paramsEnroll[AFACE_MAX_TEMPLATES] = maxTemplates;
		paramsEnroll[AFACE_CONCATENATE_MODE] = concatenateMode;
		paramsEnroll[AFACE_MIN_SCORE] = minScore;
		paramsEnroll[AFACE_MAX_SCORE] = maxScore;
	}

	JSONConfigEnroll::~JSONConfigEnroll()
	{
	}
	void JSONConfigEnroll::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == CFG_IFACE_DETECTION_THRESHOLD) {
					detectionThreshold = it.value();
					paramsEnroll[CFG_IFACE_DETECTION_THRESHOLD] = detectionThreshold;
				}
				if (it.key() == CFG_SIMILARITY_THRESHOLD) {
					similarityThreshold = it.value();
					paramsEnroll[CFG_SIMILARITY_THRESHOLD] = similarityThreshold;
				}
				if (it.key() == AFACE_GALLERY) {
					gallery = it.value();
					paramsEnroll[AFACE_GALLERY] = gallery;
				}
				if (it.key() == AFACE_DEDUPLICATE) {
					deduplicate = it.value();
					paramsEnroll[AFACE_DEDUPLICATE] = deduplicate;
				}
				if (it.key() == AFACE_CONCATENATE) {
					concatenate = it.value();
					paramsEnroll[AFACE_CONCATENATE] = concatenate;
				}
				if (it.key() == AFACE_MAX_TEMPLATES) {
					maxTemplates = it.value();
					paramsEnroll[AFACE_MAX_TEMPLATES] = maxTemplates;
				}

				if (it.key() == AFACE_CONCATENATE_MODE) {
					concatenateMode = it.value();
					paramsEnroll[AFACE_CONCATENATE_MODE] = concatenateMode;
				}
				if (it.key() == AFACE_MIN_SCORE) {
					minScore = it.value();
					paramsEnroll[AFACE_MIN_SCORE] = minScore;
				}
				if (it.key() == AFACE_MAX_SCORE) {
					maxScore = it.value();
					paramsEnroll[AFACE_MAX_SCORE] = maxScore;
				}

			}
		}
	}

	void JSONConfigEnroll::ParseToFile()
	{
		detectionThreshold = std::any_cast<int>(paramsEnroll[CFG_IFACE_DETECTION_THRESHOLD]);
		similarityThreshold = std::any_cast<int>(paramsEnroll[CFG_SIMILARITY_THRESHOLD]);
		gallery = std::any_cast<int>(paramsEnroll[AFACE_GALLERY]);
		deduplicate = std::any_cast<int>(paramsEnroll[AFACE_DEDUPLICATE]);
		concatenate = std::any_cast<int>(paramsEnroll[AFACE_CONCATENATE]);
		concatenateMode = std::any_cast<int>(paramsEnroll[AFACE_CONCATENATE_MODE]);
		maxTemplates = std::any_cast<int>(paramsEnroll[AFACE_MAX_TEMPLATES]);
		minScore = std::any_cast<int>(paramsEnroll[AFACE_MIN_SCORE]);
		maxScore = std::any_cast<int>(paramsEnroll[AFACE_MAX_SCORE]);

		nlohmann::json jParams = nlohmann::json::object({ {CFG_IFACE_DETECTION_THRESHOLD, detectionThreshold},
			{CFG_SIMILARITY_THRESHOLD, similarityThreshold}, {AFACE_GALLERY, gallery}, {AFACE_DEDUPLICATE, deduplicate},
			{AFACE_CONCATENATE, concatenate}, {AFACE_CONCATENATE_MODE, concatenateMode}, 
			{AFACE_MAX_TEMPLATES, maxTemplates}, {AFACE_MIN_SCORE, minScore}, {AFACE_MAX_SCORE, maxScore} });

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}

	std::any JSONConfigEnroll::GetParam(const char* name)
	{
		std::any result;
		if (paramsEnroll.find(name) != paramsEnroll.end())
			return paramsEnroll[name];
		return result;
	}
	void JSONConfigEnroll::SetParam(const char* name, std::any value)
	{
		paramsEnroll[name] = value;
	}
	void JSONConfigEnroll::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;

	}
	void JSONConfigEnroll::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}