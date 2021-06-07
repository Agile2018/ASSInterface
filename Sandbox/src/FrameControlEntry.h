#pragma once

#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"

class FrameControlEntry {
public:
	FrameControlEntry(ASSInterface::LanguageType language);
	~FrameControlEntry();
	void Show(bool* p_open, PersonSpecification& personSpec);
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	inline void SetPersonDetected(PersonSpecification pSpec) { 
		listDetected.push_back(pSpec); 
		listShowDetected.push_back(pSpec);
	}
	inline void SetPersonTemp(PersonSpecification pSpec) { listDetectedTemp.push_back(pSpec); }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	Rx::subject<int> channelSubject;
	Rx::observable<int> observableCurrentChannel = channelSubject.get_observable();	
private:
	void ShowScreenHead();
	void ShowDetected(int index, PersonSpecification& personSpec);
	int GetIndexDetected(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
	inline void RemovePersonDetected(int index) { 
		listShowDetected.erase(listShowDetected.begin() + index); }
	void SetPersonInside(int index, PersonSpecification& personSpec);
	void SetNewsDetected();
private:
	Rx::subscriber<int> shootChannel = channelSubject.get_subscriber();
	ASSInterface::LanguageType lg;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;	
	std::vector<bool*> viewVideo;
	int indexRemove = -1;
	std::vector<PersonSpecification> listDetected;
	std::vector<PersonSpecification> listShowDetected;
	std::vector<PersonSpecification> listDetectedTemp;
	ASSInterface::Ref<ASSInterface::Texture2D> txtButtom;
	const char* currentChannel = NULL;
	std::mutex mutexList;	
};