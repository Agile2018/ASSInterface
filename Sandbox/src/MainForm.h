#pragma once
#include <ASSInterface.h>
#include "FrameVideo.h"
#include "FrameConfiguration.h"
#include "FrameControlEntry.h"
#include "FrameEnrollment.h"
#include "FrameUnidentified.h"
#include "FrameOutputControl.h"
#include "FrameControlTracking.h"
#include "FrameEditRegister.h"
#include "FrameImport.h"

class MainForm : public ASSInterface::Layer
{
public:

	MainForm();
	~MainForm();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(ASSInterface::Timestep ts) override;
	void OnEvent(ASSInterface::Event& event) override;
	virtual void OnImGuiRender() override;
	
private:
	void InitResources();
	void InitForms();
	void ShowMenuMain();
	void ShowMenuConfiguration(bool* p_open);
	void ShowVideo(bool* p_open, int channel);
	void ShowMenuEnrollmentRAV(bool* p_open);
	void ShowMenuControlEntry(bool* p_open);	
	void ShowStatusBar();
	void ShowWindowTip(bool* p_open);
	static void ShowAppLog(bool* p_open);	
	void ObserverChannelTask();
	void ObserverIdentification();
	void ObserverTextureDocument();
	void ObserverConnection();
	void ObserverChangeConfiguration();
	void ObserverPersonOutside();
	void DefinePipe(PersonSpecification specPerson);
	void CloseConnections();
	bool FramesOpen();
	void ResetView(bool* p_open);
private:
	FrameVideo* videos = new FrameVideo[MAX_CHANNELS];
	FrameControlTracking* frmTracking = new FrameControlTracking[MAX_CHANNELS - 1];
	FrameConfiguration* frmConfiguration = nullptr;
	FrameControlEntry* frmControlEntry = nullptr;
	FrameEnrollment* frmEnrollment = nullptr;
	FrameUnidentified* frmUnidentified = new FrameUnidentified();
	FrameOutputControl* frmOutputControl = new FrameOutputControl();
	FrameEditRegister* frmEditRegister = nullptr;
	FrameImport* frmImport = nullptr;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	//ASSInterface::Ref<ASSInterface::Texture2D> txtWindowTip;
	ASSInterface::Ref<ASSInterface::File> managerFile;
	ASSInterface::Ref<ASSInterface::InitLibrary> initInnovatrics;
	ASSInterface::LanguageType lg;
	PersonSpecification personSpecificationTemp;
	int indexUnidentified = -1;	
	const std::string prgBar = "Working %c";
	std::string* messageStatus = new std::string();
	std::string progressBar = "";
	std::string nameOperator = "Angel Martinez";
	bool stateFrmTrack[MAX_CHANNELS - 1] = {};
	bool stateVideo[MAX_CHANNELS] = {};
	bool isSave = false;
	//Temp
	clock_t timeStartWindowTip;
	/*std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution{ 0, 6 };*/
};
