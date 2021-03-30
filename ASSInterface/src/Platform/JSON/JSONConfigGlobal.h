#pragma once

#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigGlobal : public Configuration {
	public:
		JSONConfigGlobal();
		virtual ~JSONConfigGlobal();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "globals.txt";
		const std::string GLOBAL_MIN_VALID_IMAGE_SIZE = "GLOBAL_MIN_VALID_IMAGE_SIZE";
		const std::string GLOBAL_GPU_DEVICE_ID = "GLOBAL_GPU_DEVICE_ID";
		const std::string GLOBAL_GPU_ENABLED = "GLOBAL_GPU_ENABLED";
		const std::string GLOBAL_THREAD_MANAGEMENT_MODE = "GLOBAL_THREAD_MANAGEMENT_MODE";
		const std::string GLOBAL_THREAD_NUM = "GLOBAL_THREAD_NUM";
		const std::string GLOBAL_CFG_LOG_LEVEL = "GLOBAL_CFG_LOG_LEVEL";

		int minValidImageSize = 200;
		int gpuDeviceId = 0;
		int gpuEnabled = 1;
		int threadManagementMode = 1;
		int threadNum = 2;
		int logLevel = 0;

		mutable std::unordered_map<std::string, std::any> paramsGlobals;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}