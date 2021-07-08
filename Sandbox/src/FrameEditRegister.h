#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"
#include "tbb/tbb.h"

using namespace tbb;

#pragma warning( disable: 588)

class FrameEditRegister {
public:
	FrameEditRegister(ASSInterface::LanguageType language);
	~FrameEditRegister();
	void Show(bool* p_open);
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	void Search();
	void Save();
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
	void ResetListFind(bool close);
	inline void SetInnoTask(ASSInterface::Ref<ASSInterface::ExecuteTask> task) { 
		innoTask = task; ObserverTask(); }
	inline bool GetStateTask() { return innoTask != nullptr; }
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	inline Rx::observable<int> GetChannel() {
		return observableCurrentChannel;		
	}
	inline void SetIndexChannel(int index) { currentIndexVideo = index; }
	inline Rx::observable<int> GetTextDocumentObverse() {
		return observableTextureDocumentObverse;
	}

	inline Rx::observable<int> GetTextDocumentReverse() {
		return observableTextureDocumentReverse;
	}
	void SetDocumentObverse(std::vector<unsigned char> dataDocument,
		int width, int height);
	void SetDocumentReverse(std::vector<unsigned char> dataDocument,
		int width, int height);
private:
	void BuildListOfSearch();
	void BuildListTemplates();
	void SearchByDocument();
	void SearchByField();
	void TransforToPersonSpecification();
	void AddPersonToList(ASSInterface::EntitySpecification specEnt);
	void Remove();
	bool VerifyId();
	void SetTemplates();
	void FootViewTemplates();
	void OpenFile(int id, int task, int index = -1);
	void ClearTask();
	void CreateTaskBiometric(int channel);
	void ObserverDetected();
	void ObserverTask();
	std::vector<std::string> Split(const std::string& s, char delimiter);
	int GetIndexTemplate(std::string nameControl);
	void RemoveFace(int index);
	void UpdateIndexDatabase();
	void SetTextureDocument();
	void OpenFileDocument();
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

	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	PersonSpecification personSpecificationEdit;
	ASSInterface::LanguageType lg;
	ASSInterface::Ref<ASSInterface::Base64> base64;	
	ASSInterface::Ref<ASSInterface::ExecuteTask> innoTask = nullptr;
	ASSInterface::Ref<ASSInterface::Detection> innoDetect = nullptr;
	ASSInterface::Ref<ASSInterface::TransformImage> transImage;
	ASSInterface::EntitySpecification m_EntitySpec;	
	const char* currentItemPersonType = NULL;
	const char* currentItemPersonType1 = NULL;
	std::string* messageStatus;
	concurrent_vector<PersonSpecification> listPersons;
	std::vector<ASSInterface::TemplateSpecification> listTemplates;
	bool viewTemplates = false;
	int currentIndexVideo = -1;
	bool isFileButton = false;
	bool isVideoButton = false;
	const char* currentChannel = NULL;
	std::vector<bool*> viewVideo;
	int docFace = 1;
};
