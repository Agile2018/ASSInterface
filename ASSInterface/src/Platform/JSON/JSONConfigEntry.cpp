#include "hzpch.h"
#include "JSONConfigEntry.h"

namespace ASSInterface {
	JSONConfigEntry::JSONConfigEntry()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsEntry[CFG_IDENTIFICATION_SIMILARITY_THRESHOLD] = identificationSimilarityThreshold;
		paramsEntry[CFG_VERIFICATION_SIMILARITY_THRESHOLD] = verificationSimilarityThreshold;
		paramsEntry[CFG_BEST_CANDIDATES_COUNT] = bestCandidates;
		paramsEntry[CFG_IDENTIFICATION_SPEED] = identificationSpeed;
	}
	JSONConfigEntry::~JSONConfigEntry()
	{
	}
	void JSONConfigEntry::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == CFG_IDENTIFICATION_SIMILARITY_THRESHOLD) {
					identificationSimilarityThreshold = it.value();
					paramsEntry[CFG_IDENTIFICATION_SIMILARITY_THRESHOLD] = identificationSimilarityThreshold;
				}
				if (it.key() == CFG_VERIFICATION_SIMILARITY_THRESHOLD) {
					verificationSimilarityThreshold = it.value();
					paramsEntry[CFG_VERIFICATION_SIMILARITY_THRESHOLD] = verificationSimilarityThreshold;
				}
				if (it.key() == CFG_BEST_CANDIDATES_COUNT) {
					bestCandidates = it.value();
					paramsEntry[CFG_BEST_CANDIDATES_COUNT] = bestCandidates;
				}
				if (it.key() == CFG_IDENTIFICATION_SPEED) {
					identificationSpeed = it.value();
					paramsEntry[CFG_IDENTIFICATION_SPEED] = identificationSpeed;
				}

			}
		}
	}
	void JSONConfigEntry::ParseToFile()
	{
		identificationSimilarityThreshold = std::any_cast<int>(paramsEntry[CFG_IDENTIFICATION_SIMILARITY_THRESHOLD]);
		verificationSimilarityThreshold = std::any_cast<int>(paramsEntry[CFG_VERIFICATION_SIMILARITY_THRESHOLD]);
		bestCandidates = std::any_cast<int>(paramsEntry[CFG_BEST_CANDIDATES_COUNT]);
		identificationSpeed = std::any_cast<int>(paramsEntry[CFG_IDENTIFICATION_SPEED]);

		nlohmann::json jParams = nlohmann::json::object({ {CFG_IDENTIFICATION_SIMILARITY_THRESHOLD, identificationSimilarityThreshold },
			{CFG_VERIFICATION_SIMILARITY_THRESHOLD, verificationSimilarityThreshold }, {CFG_BEST_CANDIDATES_COUNT, bestCandidates }, 
			{CFG_IDENTIFICATION_SPEED, identificationSpeed } });

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}
	std::any JSONConfigEntry::GetParam(const char* name)
	{
		std::any result;
		if (paramsEntry.find(name) != paramsEntry.end())
			return paramsEntry[name];
		return result;
	}
	void JSONConfigEntry::SetParam(const char* name, std::any value)
	{
		paramsEntry[name] = value;
	}
	void JSONConfigEntry::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigEntry::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}