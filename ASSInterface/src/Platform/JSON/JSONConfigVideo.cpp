#include "hzpch.h"
#include "JSONConfigVideo.h"

namespace ASSInterface {
	JSONConfigVideo::JSONConfigVideo(std::string nameFile)
	{
		fileConfig = nameFile;
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsVideo[SOURCEFLOW] = sourceFlow;
		paramsVideo[IPCAMERA] = ipCamera;
		paramsVideo[FILEVIDEO] = nameVideo;
		paramsVideo[DEVICEVIDEO] = deviceVideo;
		paramsVideo[LABELWINDOW] = lblWindow;
	}
	JSONConfigVideo::~JSONConfigVideo()
	{
	}
	void JSONConfigVideo::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == SOURCEFLOW) {
					sourceFlow = it.value();
					paramsVideo[SOURCEFLOW] = sourceFlow;
				}
				if (it.key() == IPCAMERA) {
					ipCamera = it.value();
					paramsVideo[IPCAMERA] = ipCamera;
				}
				if (it.key() == FILEVIDEO) {
					nameVideo = it.value();
					paramsVideo[FILEVIDEO] = nameVideo;
				}
				if (it.key() == DEVICEVIDEO) {
					deviceVideo = it.value();
					paramsVideo[DEVICEVIDEO] = deviceVideo;
				}
				if (it.key() == LABELWINDOW) {
					lblWindow = it.value();
					paramsVideo[LABELWINDOW] = lblWindow;
				}
				
			}

		}
		
	}

	void JSONConfigVideo::ParseToFile()
	{
		ipCamera = std::any_cast<std::string>(paramsVideo[IPCAMERA]);
		nameVideo = std::any_cast<std::string>(paramsVideo[FILEVIDEO]);
		deviceVideo = std::any_cast<std::string>(paramsVideo[DEVICEVIDEO]);
		lblWindow = std::any_cast<std::string>(paramsVideo[LABELWINDOW]);
		sourceFlow = std::any_cast<int>(paramsVideo[SOURCEFLOW]);

		nlohmann::json jParams = nlohmann::json::object({ {SOURCEFLOW, sourceFlow}, 
			{IPCAMERA, ipCamera}, {FILEVIDEO, nameVideo}, {DEVICEVIDEO, deviceVideo}, {LABELWINDOW, lblWindow} });
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
	void JSONConfigVideo::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;		
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigVideo::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}