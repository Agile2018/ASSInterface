#pragma once


#include <ASSInterface.h>
#include "FrameVideo.h"
#include "FrameConfiguration.h"
#include "FrameControlEntry.h"
#include "FrameEnrollment.h"
#include "FrameUnidentified.h"
#include "FrameOutputControl.h"

class MainForm : public ASSInterface::Layer
{
public:

	MainForm();
	virtual ~MainForm() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(ASSInterface::Timestep ts) override;
	void OnEvent(ASSInterface::Event& event) override;
	virtual void OnImGuiRender() override;

private:
	void ShowMenuMain();
	void ShowMenuConfiguration(bool* p_open);
	void ShowVideo(bool* p_open, int channel);
	void ShowMenuEnrollment(bool* p_open);
	void ShowMenuControlEntry(bool* p_open);	
	void ShowStatusBar();
	void ShowWindowTip(bool* p_open);

	//Temp
	//bool SaveDataPerson(int channel);
	void SetImageToTexture(const std::string& path, ASSInterface::Ref<ASSInterface::Texture2D>& texture);
	void SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel, 
		ASSInterface::Ref<ASSInterface::Texture2D>& texture);	
	
	//void GetDataPerson(std::string id);	
	void LoadUnidentified();
	

	FrameVideo* videos = new FrameVideo[MAX_CHANNELS];	
	FrameConfiguration* frmConfiguration = nullptr;
	FrameControlEntry* frmControlEntry = new FrameControlEntry();
	FrameEnrollment* frmEnrollment = new FrameEnrollment();		
	FrameUnidentified* frmUnidentified = new FrameUnidentified();
	FrameOutputControl* frmOutputControl = new FrameOutputControl();
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	ASSInterface::Ref<ASSInterface::Texture2D> txtWindowTip;
	//ASSInterface::Ref<ASSInterface::TransformImage> transformImage;
	//ASSInterface::Ref<ASSInterface::Base64> base64;	
	ASSInterface::Ref<ASSInterface::File> managerFile;
	ASSInterface::LanguageType lg; 	
	//PersonSpecification personSpecificationLastAction;
	PersonSpecification personSpecificationDetect;	
private:
	int indexUnidentified = -1;	
	const std::string prgBar = "Working %c";
	std::string messageStatus = "Welcome";
	std::string progressBar = "";

	//Temp
	clock_t timeStartWindowTip;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution{ 0, 6 };
};
