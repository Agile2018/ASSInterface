#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigVideo : public Configuration
	{
	public:
		JSONConfigVideo(std::string nameFile);
		virtual ~JSONConfigVideo();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;		
		int sourceFlow = 1;
		std::string ipCamera = "rtsp://root:123qwe@192.168.0.12:554/axis-media/media.amp";
		std::string nameVideo = "assets/textures/video2.mp4";
		std::string deviceVideo = "/dev/video0"; //
		std::string fileVideo = "video.txt";
		const std::string SOURCEFLOW = "sourceFlow";
		const std::string IPCAMERA = "ipCamera";
		const std::string FILEVIDEO = "fileVideo";
		const std::string DEVICEVIDEO = "deviceVideo";
		mutable std::unordered_map<std::string, std::any> paramsVideo;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}