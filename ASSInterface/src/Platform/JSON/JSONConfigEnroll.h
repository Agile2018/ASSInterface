#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigEnroll : public Configuration {
	public:
		JSONConfigEnroll();
		virtual ~JSONConfigEnroll();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "enroll.txt";

		int detectionThreshold = 400;
		int similarityThreshold = 40;
		int gallery = 0;
		int deduplicate = 1;
		int concatenate = 0;
		int maxTemplates = 10;
		int concatenateMode = 1;
		int minScore = 50;
		int maxScore = 90;
		int duration = 30;

		const std::string CFG_IFACE_DETECTION_THRESHOLD = "CFG_IFACE_DETECTION_THRESHOLD";
		const std::string CFG_SIMILARITY_THRESHOLD = "CFG_SIMILARITY_THRESHOLD";
		const std::string AFACE_GALLERY = "AFACE_GALLERY";
		const std::string AFACE_DEDUPLICATE = "AFACE_DEDUPLICATE";
		const std::string AFACE_CONCATENATE = "AFACE_CONCATENATE";
		const std::string AFACE_MAX_TEMPLATES = "AFACE_MAX_TEMPLATES";
		const std::string AFACE_CONCATENATE_MODE = "AFACE_CONCATENATE_MODE";
		const std::string AFACE_MIN_SCORE = "AFACE_MIN_SCORE";
		const std::string AFACE_MAX_SCORE = "AFACE_MAX_SCORE";
		const std::string AFACE_DURATION = "AFACE_DURATION";

		mutable std::unordered_map<std::string, std::any> paramsEnroll;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}