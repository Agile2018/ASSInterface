#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigOnTheFly : public Configuration {
	public:
		JSONConfigOnTheFly();
		virtual ~JSONConfigOnTheFly();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "rav.txt";

		const std::string AFACE_NUMBER_TEMPLATES = "AFACE_NUMBER_TEMPLATES";
		const std::string AFACE_NUMBER_IMAGES = "AFACE_NUMBER_IMAGES";

		int numberTemplates = 1, showNumberImages = 1;

		mutable std::unordered_map<std::string, std::any> paramsOnTheFly;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}