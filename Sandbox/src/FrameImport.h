#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
#include "data/ConstantApplication.h"
#include "tbb/tbb.h"

using namespace tbb;

class FrameImport {
public:
	FrameImport(ASSInterface::LanguageType language);
	~FrameImport();
	void Show(bool* p_open);
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	inline void ResetChannel() { currentIndexVideo = -1; }
private:
	void OpenFile();
	void CreateTaskEnroll(int channel);
	void ObserverDetected();
	void ObserverTask();
	void BranchImport(std::vector<std::string> filePaths);
private:
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	ASSInterface::Ref<ASSInterface::ExecuteTask> innoTask = nullptr;
	ASSInterface::Ref<ASSInterface::Detection> innoDetect = nullptr;
	ASSInterface::LanguageType lg;
	int currentIndexVideo = -1;
	std::string* messageStatus = nullptr;
	std::vector<bool*> viewVideo;
	const char* currentChannel = NULL;
	const char* currentItemPersonType = NULL;
};
