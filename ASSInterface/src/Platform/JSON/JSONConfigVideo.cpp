#include "hzpch.h"
#include "JSONConfigVideo.h"

namespace ASSInterface {
	JSONConfigVideo::JSONConfigVideo(std::string nameFile)
	{
		fileVideo = nameFile;
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileVideo;
		paramsVideo[SOURCEFLOW] = sourceFlow;
		paramsVideo[IPCAMERA] = ipCamera;
		paramsVideo[FILEVIDEO] = nameVideo;
		paramsVideo[DEVICEVIDEO] = deviceVideo;
	}
	JSONConfigVideo::~JSONConfigVideo()
	{
	}
	void JSONConfigVideo::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		auto jdata = nlohmann::json::parse(strJson);
		sourceFlow = jdata[SOURCEFLOW];
		ipCamera = jdata[IPCAMERA];
		nameVideo = jdata[FILEVIDEO];
		deviceVideo = jdata[DEVICEVIDEO];

		paramsVideo[SOURCEFLOW] = sourceFlow;
		paramsVideo[IPCAMERA] = ipCamera;
		paramsVideo[FILEVIDEO] = nameVideo;
		paramsVideo[DEVICEVIDEO] = deviceVideo;
	}

	void JSONConfigVideo::ParseToFile()
	{
		nlohmann::json jParams = nlohmann::json::object({ {SOURCEFLOW, sourceFlow}, 
			{IPCAMERA, ipCamera}, {FILEVIDEO, nameVideo}, {DEVICEVIDEO, deviceVideo} });
		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}

	std::any JSONConfigVideo::GetParam(const char* name)
	{
		std::any result;
		if (paramsVideo.find(name) != paramsVideo.end())
			return paramsVideo[name];
		return result;
	}

	void JSONConfigVideo::SetParam(const char* name, std::any value)
	{
		paramsVideo[name] = value;
	}
	void JSONConfigVideo::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}