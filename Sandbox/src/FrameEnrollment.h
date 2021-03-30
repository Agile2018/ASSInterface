#pragma once
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameEnrollment {
public:
	FrameEnrollment();
	~FrameEnrollment();
	void Show(bool* p_open, PersonSpecification& personSpec);
	inline PersonSpecification* GetPersonEnrolled() { return &personSpecificationEnroll; }
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	inline void SetEnabledCapture(bool value) { buttonsCaptureEnroll = value; }
	void SetImageToTexture(const std::string& path);
	void SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel);	
	//inline bool GetIsSavePerson() { return savePerson; }
	inline int GetIndexCurrentVideo() { return currentIndexVideo; }
	//inline void SetIsSavePerson(bool value) { savePerson = false; }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	void SetBufferCapture(std::vector<unsigned char> buff);
	void CleanSpecification();
private:
	PersonSpecification personSpecificationEnroll;
	ASSInterface::Ref<ASSInterface::Base64> base64;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	std::vector<bool*> viewVideo;
	const char* itemsTypePerson[3] = { "Visitors", "Observed", "Employees" };
	const char* itemsVideo[2] = { "Video 1", "Video 2" };
	const char* itemsTarget[2] = { "Office 1", "Office 2" };
	bool buttonsCaptureEnroll = true;	
	int currentIndexVideo = -1;	
	bool readOnlyDate = true;
private:
	void OpenFile();
	void SavePerson(PersonSpecification& personSpec);
};