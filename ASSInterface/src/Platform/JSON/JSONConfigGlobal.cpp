#include "hzpch.h"
#include "JSONConfigGlobal.h"

namespace ASSInterface {
	JSONConfigGlobal::JSONConfigGlobal()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsGlobals[GLOBAL_MIN_VALID_IMAGE_SIZE] = minValidImageSize;
		paramsGlobals[GLOBAL_GPU_DEVICE_ID] = gpuDeviceId;
		paramsGlobals[GLOBAL_GPU_ENABLED] = gpuEnabled;
		paramsGlobals[GLOBAL_THREAD_MANAGEMENT_MODE] = threadManagementMode;
		paramsGlobals[GLOBAL_THREAD_NUM] = threadNum;
		paramsGlobals[GLOBAL_CFG_LOG_LEVEL] = logLevel;
	}
	JSONConfigGlobal::~JSONConfigGlobal()
	{
	}
	void JSONConfigGlobal::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);		

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == GLOBAL_MIN_VALID_IMAGE_SIZE) {
					minValidImageSize = it.value();
					paramsGlobals[GLOBAL_MIN_VALID_IMAGE_SIZE] = minValidImageSize;
				}
				if (it.key() == GLOBAL_GPU_DEVICE_ID) {
					gpuDeviceId = it.value();
					paramsGlobals[GLOBAL_GPU_DEVICE_ID] = gpuDeviceId;
				}
				if (it.key() == GLOBAL_GPU_ENABLED) {
					gpuEnabled = it.value();
					paramsGlobals[GLOBAL_GPU_ENABLED] = gpuEnabled;
				}
				if (it.key() == GLOBAL_THREAD_MANAGEMENT_MODE) {
					threadManagementMode = it.value();
					paramsGlobals[GLOBAL_THREAD_MANAGEMENT_MODE] = threadManagementMode;
				}
				if (it.key() == GLOBAL_THREAD_NUM) {
					threadNum = it.value();
					paramsGlobals[GLOBAL_THREAD_NUM] = threadNum;
				}
				if (it.key() == GLOBAL_CFG_LOG_LEVEL) {
					logLevel = it.value();
					paramsGlobals[GLOBAL_CFG_LOG_LEVEL] = logLevel;
				}
			}
		}
	}

	void JSONConfigGlobal::ParseToFile()
	{
		minValidImageSize = std::any_cast<int>(paramsGlobals[GLOBAL_MIN_VALID_IMAGE_SIZE]);
		gpuDeviceId = std::any_cast<int>(paramsGlobals[GLOBAL_GPU_DEVICE_ID]);
		gpuEnabled = std::any_cast<int>(paramsGlobals[GLOBAL_GPU_ENABLED]);
		threadManagementMode = std::any_cast<int>(paramsGlobals[GLOBAL_THREAD_MANAGEMENT_MODE]);
		threadNum = std::any_cast<int>(paramsGlobals[GLOBAL_THREAD_NUM]);
		logLevel = std::any_cast<int>(paramsGlobals[GLOBAL_CFG_LOG_LEVEL]);

		nlohmann::json jParams = nlohmann::json::object({ {GLOBAL_MIN_VALID_IMAGE_SIZE, minValidImageSize},
			{GLOBAL_GPU_DEVICE_ID, gpuDeviceId}, {GLOBAL_GPU_ENABLED, gpuEnabled}, 
			{GLOBAL_THREAD_MANAGEMENT_MODE, threadManagementMode}, {GLOBAL_THREAD_NUM, threadNum}, 
			{GLOBAL_CFG_LOG_LEVEL, logLevel} });

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}

	std::any JSONConfigGlobal::GetParam(const char* name)
	{
		std::any result;
		if (paramsGlobals.find(name) != paramsGlobals.end())
			return paramsGlobals[name];
		return result;
	}

	void JSONConfigGlobal::SetParam(const char* name, std::any value)
	{
		paramsGlobals[name] = value;
	}

	void JSONConfigGlobal::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigGlobal::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}