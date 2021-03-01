#pragma once

#include <ASSInterface.h>
#include "FrameVideo.h"

struct PersonSpecification {
	char name[32] = "Name";
	char document[32] = "123456";
	char date[32] = "02102021";
	char id[15] = "1";
	char match[4] = "70%";
	char state[20] = "Autorized";
	char place[10] = "8vo";
	char channel[2] = "1";
	char type[20] = "Visitor";
	ASSInterface::Ref<ASSInterface::Texture2D> txtCapture;
	ASSInterface::Ref<ASSInterface::Texture2D> txtGallery;

};

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
	void SaveDataPerson(int channel);
	void SetImageToTexture(const std::string& path, ASSInterface::Ref<ASSInterface::Texture2D>& texture);
	void SetImageToTexture(void* data, uint32_t width, uint32_t height, uint32_t channel, 
		ASSInterface::Ref<ASSInterface::Texture2D>& texture);
	void OpenFile();
	void GetDataPerson(std::string id);
	void ShowScreenUnidentified();
	void LoadUnidentified();
	void ShowScreenHeadControlEntry();
	void ShowScreenLastAction();
	void ShowScreenDetect();
	FrameVideo* videos = new FrameVideo[MAX_CHANNELS];	
	ASSInterface::Ref<ASSInterface::Texture2D> txtImage;	
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	ASSInterface::Ref<ASSInterface::TransformImage> transformImage;
	ASSInterface::Ref<ASSInterface::Base64> base64;
	ASSInterface::Ref<ASSInterface::Configuration> configurationDB;
	ASSInterface::Ref<ASSInterface::File> managerFile;
	PersonSpecification personSpecificationEnroll;
	PersonSpecification personSpecificationLastAction;
	PersonSpecification personSpecificationDetect;
	std::vector<PersonSpecification> listUnidentified;
private:
	bool readOnly = false;	
	//uint32_t txtImage1;
};
