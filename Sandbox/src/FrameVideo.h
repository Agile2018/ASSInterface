#pragma once

#include <ASSInterface.h>

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
	inline std::vector<unsigned char> GetDataRaw() { return m_Streamer->GetDataRaw(); }
	inline int GetWidthImage() { return m_Streamer->GetWidth(); }
	inline int GetHeightImage() { return m_Streamer->GetHeight(); }	
private:
	void RefreshBox();
	void RunFlow();
	void CreateBuffer();
	void InitTexture();
private:
	const std::string threadInit = "thrinit";
	const std::string threadRun = "thrrun";
	typedef std::unordered_map<std::string, std::thread> ThreadMap;
	ThreadMap tm_;

	ASSInterface::ShaderLibrary m_ShaderLibrary;	
	ASSInterface::Ref<ASSInterface::VertexArray> vertexArrayTexture;
	ASSInterface::Ref<ASSInterface::Texture2D> m_Texture;
	ASSInterface::OrthographicCameraController m_CameraController;
	ASSInterface::Ref<ASSInterface::Framebuffer> m_Framebuffer;
	ASSInterface::Ref<ASSInterface::ASSStreamer> m_Streamer;
	float heigthTexture = 0.0f;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	bool m_ViewportFocused = false, m_ViewportHovered = false, isBufferCreate = false, flowRunning = false;

	/// Box line


	ASSInterface::Ref<ASSInterface::VertexArray> vertexArrayBox;
	ASSInterface::Ref<ASSInterface::Shader> shaderBox;	
	ASSInterface::Ref<ASSInterface::IndexBuffer> indexBufferBox;
	std::vector<float> verticesBox;
	std::vector<uint32_t> indicesBox;
	float x = 0.0f, y = 0.0f, ratio = 0.15f;
	unsigned short pattern = 0xAAAA; //0x18ff 0xAAAA
	float factor = 4.0f, elapse = 0.0f;
	std::string nameWindow = "Video 1";
	
	//Temp
	bool check = false;
};
