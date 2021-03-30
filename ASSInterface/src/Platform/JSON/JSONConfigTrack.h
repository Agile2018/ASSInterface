#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigTrack : public Configuration {
	public:
		JSONConfigTrack();
		virtual ~JSONConfigTrack();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "track.txt";

		int trackingMode = 2;
		int trackSpeed = 2;
		int motionOptimization = 2;
		int discoveryFrequenceMS = 2000;
		bool deepTrack = true;
		int countRedetectTimeDelta = 10000;

		const std::string FACE_DISCOVERY_FREQUENCE_MS = "TRACK_FACE_DISCOVERY_FREQUENCE_MS";
		const std::string COUNT_REDETECT_TIME_DELTA = "COUNT_REDETECT_TIME_DELTA";
		const std::string TRACK_TRACKING_MODE = "TRACK_TRACKING_MODE";
		const std::string TRACK_SPEED_ACCURACY_MODE = "TRACK_SPEED_ACCURACY_MODE";
		const std::string TRACK_MOTION_OPTIMIZATION = "TRACK_MOTION_OPTIMIZATION";
		const std::string TRACK_DEEP_TRACK = "TRACK_DEEP_TRACK";

		mutable std::unordered_map<std::string, std::any> paramsTrack;
		ASSInterface::Ref<ASSInterface::File> managerFile;
	};
}