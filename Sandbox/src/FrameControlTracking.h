#pragma once

#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"

class FrameControlTracking {
public:
	FrameControlTracking();
	~FrameControlTracking();
	void Show(bool* p_open, int index);
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }	
	inline void SetPersonTemp(PersonSpecification pSpec) { listDetectedTemp.push_back(pSpec); }
	inline Rx::observable<std::tuple<int, int>> GetChannel(){
		return observableCurrentChannel;
	}
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
private:
	void ShowScreenHead(int index);
	void ShowDetected(int index);
	void SetNewsDetected();
	void SaveEventTrack(PersonSpecification& personSpec);
	void SaveEventTrackUnidentified(PersonSpecification& personSpec);
private:
	Rx::subject<std::tuple<int, int>> channelSubject;
	Rx::observable<std::tuple<int, int>> observableCurrentChannel = channelSubject.get_observable();
	Rx::subscriber<std::tuple<int, int>> shootChannel = channelSubject.get_subscriber();
	std::vector<bool*> viewVideo;
	std::vector<PersonSpecification> listDetected;
	std::vector<PersonSpecification> listDetectedTemp;
	ASSInterface::Ref<ASSInterface::Texture2D> txtButtom;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	bool popup_open = false;
	const char* current_item = NULL;
	std::mutex mutexList;
};