#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"

class FrameEnrollment {
public:
	FrameEnrollment(ASSInterface::LanguageType language);
	~FrameEnrollment();
	void Show(bool* p_open, PersonSpecification& personSpec);
	void ShowRAV(bool* p_open, PersonSpecification& personSpec);
	inline PersonSpecification* GetPersonEnrolled() { return &personSpecificationEnroll; }
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	//inline void SetEnabledCapture(bool value) { buttonsCaptureEnroll = value; }
	//void SetImageToTexture(const std::string& path);
	//void SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel);		
	inline int GetIndexCurrentVideo() { return currentIndexVideo; }	
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }	
	void CleanSpecification(int option);
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
	inline void SetInnoTask(ASSInterface::Ref<ASSInterface::ExecuteTask> task) { innoTask = task; }
	void ResetChannel();	
	void ClearTask();
	inline void ResetButtonVideo() { isVideoButton = false; currentIndexVideo = -1; }
	inline Rx::observable<int> GetChannel() {
		return observableCurrentChannel;
	}
	//inline ASSInterface::Ref<ASSInterface::ExecuteTask> GetTask() { return innoTask; }
	inline void SetDataEnrollTemp(PersonSpecification specPerson) { 
		listDetected.push_back(specPerson); };
	inline Rx::observable<int> GetTextDocumentObverse() {
		return observableTextureDocumentObverse;
	}

	inline Rx::observable<int> GetTextDocumentReverse() {
		return observableTextureDocumentReverse;
	}

	inline Rx::observable<bool> CloseConnection() {
		return observableClose;
	}

	void SetDocumentObverse(std::vector<unsigned char> dataDocument,
		int width, int height);
	void SetDocumentReverse(std::vector<unsigned char> dataDocument,
		int width, int height);

private:
	Rx::subject<int> channelSubject;
	Rx::observable<int> observableCurrentChannel = channelSubject.get_observable();
	Rx::subscriber<int> shootChannel = channelSubject.get_subscriber();

	Rx::subject<int> textDocumentSubjectObverse;
	Rx::observable<int> observableTextureDocumentObverse = textDocumentSubjectObverse.get_observable();
	Rx::subscriber<int> shootRequireObverse = textDocumentSubjectObverse.get_subscriber();

	Rx::subject<int> textDocumentSubjectReverse;
	Rx::observable<int> observableTextureDocumentReverse = textDocumentSubjectReverse.get_observable();
	Rx::subscriber<int> shootRequireReverse = textDocumentSubjectReverse.get_subscriber();

	Rx::subject<bool> closeSubjectConnection;
	Rx::observable<bool> observableClose = closeSubjectConnection.get_observable();
	Rx::subscriber<bool> shootCloseConnection = closeSubjectConnection.get_subscriber();

	PersonSpecification personSpecificationEnroll;
	PersonSpecification personSpecificationEnrollComplete;
	ASSInterface::Ref<ASSInterface::Base64> base64;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	ASSInterface::Ref<ASSInterface::ExecuteTask> innoTask = nullptr;
	ASSInterface::Ref<ASSInterface::Detection> innoDetect = nullptr;
	ASSInterface::Ref<ASSInterface::Texture2D> txtButtonCapture;
	ASSInterface::Ref<ASSInterface::Texture2D> txtButtonDelete;		
	ASSInterface::Ref<ASSInterface::Texture2D> txtDocumentObverse;
	ASSInterface::Ref<ASSInterface::Texture2D> txtDocumentReverse;
	std::vector<bool*> viewVideo;	
	bool isFileButton = false;	
	bool isVideoButton = false;
	bool isEnrollRAV = false;
	int currentIndexVideo = -1, widthDoc, heightDoc, channelsDoc;
	std::string* messageStatus;
	ASSInterface::LanguageType lg;
	const char* currentChannel = NULL;
	std::vector<PersonSpecification> listDetected;
private:
	void OpenFile();
	bool SavePerson(PersonSpecification& personSpec);
	bool SavePerson();
	void SaveEventRegister(PersonSpecification& personSpec);
	void SaveEventInside(PersonSpecification& personSpec);
	void CreateTaskEnroll(int channel);
	void ObserverEnroll();
	void SetDataEnrollFace(ASSInterface::IdentitySpecification specIdentify);
	void ObserverDetected();
	void SetTextureNewDetected();
	bool SaveDatabase();
	bool SaveFaces();
	void RemoveFace(int id);
	void SetTextureDocument();
};