#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigEntry : public Configuration {
	public:
		JSONConfigEntry();
		virtual ~JSONConfigEntry();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "entry.txt";

		const std::string CFG_IDENTIFICATION_SIMILARITY_THRESHOLD = "CFG_IDENTIFICATION_SIMILARITY_THRESHOLD";
		const std::string CFG_VERIFICATION_SIMILARITY_THRESHOLD = "CFG_VERIFICATION_SIMILARITY_THRESHOLD";
		const std::string CFG_BEST_CANDIDATES_COUNT = "CFG_BEST_CANDIDATES_COUNT";
		const std::string CFG_IDENTIFICATION_SPEED = "CFG_IDENTIFICATION_SPEED";

		int identificationSimilarityThreshold = 60;
		int verificationSimilarityThreshold = 60;
		int bestCandidates = 3;
		int identificationSpeed = 3;

		mutable std::unordered_map<std::string, std::any> paramsEntry;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}