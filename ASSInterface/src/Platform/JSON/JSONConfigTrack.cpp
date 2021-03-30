#include "hzpch.h"
#include "JSONConfigTrack.h"

namespace ASSInterface {
	JSONConfigTrack::JSONConfigTrack()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsTrack[COUNT_REDETECT_TIME_DELTA] = countRedetectTimeDelta;
		paramsTrack[FACE_DISCOVERY_FREQUENCE_MS] = discoveryFrequenceMS;
		paramsTrack[TRACK_TRACKING_MODE] = trackingMode;
		paramsTrack[TRACK_SPEED_ACCURACY_MODE] = trackSpeed;
		paramsTrack[TRACK_MOTION_OPTIMIZATION] = motionOptimization;
		paramsTrack[TRACK_DEEP_TRACK] = deepTrack;
	}

	JSONConfigTrack::~JSONConfigTrack()
	{
	}
	void JSONConfigTrack::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == COUNT_REDETECT_TIME_DELTA) {
					countRedetectTimeDelta = it.value();
					paramsTrack[COUNT_REDETECT_TIME_DELTA] = countRedetectTimeDelta;
				}
				if (it.key() == FACE_DISCOVERY_FREQUENCE_MS) {
					discoveryFrequenceMS = it.value();
					paramsTrack[FACE_DISCOVERY_FREQUENCE_MS] = discoveryFrequenceMS;
				}
				if (it.key() == TRACK_TRACKING_MODE) {
					trackingMode = it.value();
					paramsTrack[TRACK_TRACKING_MODE] = trackingMode;
				}
				if (it.key() == TRACK_SPEED_ACCURACY_MODE) {
					trackSpeed = it.value();
					paramsTrack[TRACK_SPEED_ACCURACY_MODE] = trackSpeed;
				}
				if (it.key() == TRACK_MOTION_OPTIMIZATION) {
					motionOptimization = it.value();
					paramsTrack[TRACK_MOTION_OPTIMIZATION] = motionOptimization;
				}
				if (it.key() == TRACK_DEEP_TRACK) {
					deepTrack = it.value();
					paramsTrack[TRACK_DEEP_TRACK] = deepTrack;
				}
			}
		}
	}
	void JSONConfigTrack::ParseToFile()
	{
		countRedetectTimeDelta = std::any_cast<int>(paramsTrack[COUNT_REDETECT_TIME_DELTA]);
		discoveryFrequenceMS = std::any_cast<int>(paramsTrack[FACE_DISCOVERY_FREQUENCE_MS]);
		trackingMode = std::any_cast<int>(paramsTrack[TRACK_TRACKING_MODE]);
		trackSpeed = std::any_cast<int>(paramsTrack[TRACK_SPEED_ACCURACY_MODE]);
		motionOptimization = std::any_cast<int>(paramsTrack[TRACK_MOTION_OPTIMIZATION]);
		deepTrack = std::any_cast<bool>(paramsTrack[TRACK_DEEP_TRACK]);


		nlohmann::json jParams = nlohmann::json::object({ {COUNT_REDETECT_TIME_DELTA, countRedetectTimeDelta},
			{FACE_DISCOVERY_FREQUENCE_MS, discoveryFrequenceMS}, {TRACK_TRACKING_MODE, trackingMode}, {TRACK_SPEED_ACCURACY_MODE, trackSpeed},
			{TRACK_MOTION_OPTIMIZATION, motionOptimization}, {TRACK_DEEP_TRACK, deepTrack }});

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}
	std::any JSONConfigTrack::GetParam(const char* name)
	{
		std::any result;
		if (paramsTrack.find(name) != paramsTrack.end())
			return paramsTrack[name];
		return result;
	}

	void JSONConfigTrack::SetParam(const char* name, std::any value)
	{
		paramsTrack[name] = value;
	}
	void JSONConfigTrack::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigTrack::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}