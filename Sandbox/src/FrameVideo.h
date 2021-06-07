#pragma once

#include <ASSInterface.h>
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"

class FrameVideo : public ASSInterface::Layer
{
public:
	FrameVideo();		
	virtual ~FrameVideo() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(ASSInterface::Timestep ts) override;
	void OnEvent(ASSInterface::Event & event) override;
	virtual void OnImGuiRender() override;
	void ShowVideo(bool* p_open);
	void StopThread(const std::string& tname);
	void StartThread(const std::string& tname, bool runFlow);
	void Init(int channel);
	inline bool GetFlowRunning() { return flowRunning; }
	inline const std::string GetNameThreadRun() { return threadRun; }
	//inline std::vector<unsigned char> GetDataRaw() { return m_Streamer->GetDataRaw(); }
	inline int GetWidthImage() { return m_Streamer->GetWidth(); }
	inline int GetHeightImage() { return m_Streamer->GetHeight(); }	
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
	inline bool IsCorrectStart() { return m_Streamer->IsCorrectStart(); }
	inline void SetTask(int value) { m_Streamer->SetTask(value); }
	inline int GetTask() { return m_Streamer->GetTask(); }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) {
		m_Streamer->SetDatabase(db); }
	inline const Rx::observable<PersonSpecification> GetDataPerson()
		const {return observableSpecPerson;}
	ASSInterface::Ref<ASSInterface::ExecuteTask> GetExecuteTask();
	inline void CloseConnection() { m_Streamer->CloseConnection(); }
	inline std::vector<unsigned char> GetDataTexture() {return m_Streamer->GetDataTexture(); };
private:
	void RefreshBoxes();
	void RunFlow();
	void CreateBuffer();
	void InitTexture();
	void ObserverIdentify();
	void SetSpecificationPerson(ASSInterface::IdentitySpecification& specIdentify);
private:
	const std::string threadInit = "thrinit";
	const std::string threadRun = "thrrun";
	typedef std::unordered_map<std::string, std::thread> ThreadMap;
	ThreadMap tm_;

	Rx::subject<PersonSpecification> specSubject;
	Rx::observable<PersonSpecification> observableSpecPerson = specSubject.get_observable();
	Rx::subscriber<PersonSpecification> shootSpecPerson = specSubject.get_subscriber();

	ASSInterface::ShaderLibrary m_ShaderLibrary;	
	ASSInterface::Ref<ASSInterface::VertexArray> vertexArrayTexture;
	ASSInterface::Ref<ASSInterface::Texture2D> m_Texture;
	ASSInterface::OrthographicCameraController m_CameraController;
	ASSInterface::Ref<ASSInterface::Framebuffer> m_Framebuffer;
	ASSInterface::Ref<ASSInterface::ASSStreamer> m_Streamer;	
	ASSInterface::Ref<ASSInterface::TransformImage> transImage;
	ASSInterface::Ref<ASSInterface::Base64> base64;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	bool m_ViewportFocused = false, m_ViewportHovered = false, 
		isBufferCreate = false, flowRunning = false;
	std::string* messageStatus;
	
	/// Box line

	std::vector< ASSInterface::Ref<ASSInterface::VertexArray>> vertexArrayBoxes;	
	ASSInterface::Ref<ASSInterface::Shader> shaderBox;	
	ASSInterface::Ref<ASSInterface::IndexBuffer> indexBufferBox;	
	std::vector<uint32_t> indicesBox;
	unsigned short pattern = 0xAAAA; //0x18ff 0xAAAA
	float factor = 4.0f, elapse = 0.0f;
	std::string nameWindow = "Video 1";
		
};
