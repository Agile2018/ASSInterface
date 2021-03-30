#include "hzpch.h"
#include "FrameVideo.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


FrameVideo::FrameVideo() : Layer("FrameVideo"), m_CameraController(1024.0f / 768.0f) //1280.0f / 960.0f
{
	ASS_PROFILE_FUNCTION();

	indicesBox.push_back(0);
	indicesBox.push_back(1);
	indicesBox.push_back(2);
	indicesBox.push_back(3);
	indicesBox.push_back(4);
	indicesBox.push_back(5);
	indicesBox.push_back(6);
	indicesBox.push_back(7);	

	vertexArrayBox.reset(ASSInterface::VertexArray::Create());

	verticesBox.push_back(x - ratioWidth); // BOTTOM LEFT TO BOTTOM RIGHT
	verticesBox.push_back(y - ratioHeight);
	verticesBox.push_back(x + ratioWidth);
	verticesBox.push_back(y - ratioHeight);

	verticesBox.push_back(x + ratioWidth); //BOTTOM RIGHT TO TOP RIGHT
	verticesBox.push_back(y - ratioHeight);
	verticesBox.push_back(x + ratioWidth);
	verticesBox.push_back(y + ratioHeight);

	verticesBox.push_back(x + ratioWidth); // TOP RIGHT TO TOP LEFT
	verticesBox.push_back(y + ratioHeight);
	verticesBox.push_back(x - ratioWidth);
	verticesBox.push_back(y + ratioHeight);

	verticesBox.push_back(x - ratioWidth); // TOP LEFT TO BOTTOM LEFT
	verticesBox.push_back(y + ratioHeight);
	verticesBox.push_back(x - ratioWidth);
	verticesBox.push_back(y - ratioHeight);

	ASSInterface::Ref<ASSInterface::VertexBuffer> vertexBufferBox;
	vertexBufferBox.reset(ASSInterface::VertexBuffer::Create(&verticesBox[0], (uint32_t)verticesBox.size() * sizeof(float))); //
	vertexBufferBox->SetLayout({
		{ ASSInterface::ShaderDataType::Float2, "a_Position" }
	});
	
	vertexArrayBox->AddVertexBuffer(vertexBufferBox);

	indexBufferBox.reset(ASSInterface::IndexBuffer::Create(&indicesBox[0], sizeof(indicesBox) / sizeof(uint32_t)));
	vertexArrayBox->SetIndexBuffer(indexBufferBox);
	shaderBox = m_ShaderLibrary.Load("assets/shaders/BoxShader.glsl");

	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->Bind();
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniformFloat4("u_Color", { 0.2f, 0.2f, 0.9f, 1.0f });
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniformFloat("u_factor", factor);
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniform1ui("u_pattern", pattern);
	
}

void FrameVideo::Init(int channel) {
	ASS_PROFILE_FUNCTION();	
	m_Streamer = ASSInterface::ASSStreamer::Create(channel);
	nameWindow = m_Streamer->GetNameWindow();
	StartThread(threadInit, false);
}

FrameVideo::~FrameVideo()
{
	verticesBox.clear();
	indicesBox.clear();
	m_Streamer->FinishLoop();
}

void FrameVideo::OnAttach()
{
	
}

void FrameVideo::OnDetach()
{

}

void FrameVideo::OnUpdate(ASSInterface::Timestep ts)
{
	ASS_PROFILE_FUNCTION();

	if (isBufferCreate && flowRunning)
	{
		if (ASSInterface::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

		}
	}
	

	if (m_ViewportFocused)
		m_CameraController.OnUpdate(ts);

	if (m_Streamer->GetChannels() == 3 && !isBufferCreate)
	{		
		CreateBuffer();			
		InitTexture();
		StopThread(threadInit);
		isBufferCreate = true;
		
	}

	if (flowRunning) {
		elapse += ts.GetMilliseconds();
		if (elapse >= m_Streamer->GetElapse())
		{
			elapse = 0.0f;

			std::vector<unsigned char> data = m_Streamer->GetDataTexture();
			if (!data.empty())
			{
				m_Texture->SetData(&data[0], m_Streamer->GetSizeImage());
			}
			//RefreshBox();
		}

		{
			ASS_PROFILE_SCOPE("Render Prep ");
			m_Framebuffer->Bind();
			ASSInterface::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			ASSInterface::RenderCommand::Clear();
		}

		{
			ASS_PROFILE_SCOPE("Render draw");
			ASSInterface::Renderer::BeginScene(m_CameraController.GetCamera());

			auto textureShader = m_ShaderLibrary.Get("Texture");
			m_Texture->Bind();
			ASSInterface::Renderer::Submit(textureShader, vertexArrayTexture);

			/*auto boxShader = m_ShaderLibrary.Get("BoxShader");
			std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(boxShader)->Bind();
			std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(boxShader)->UploadUniformFloat2("u_resolution",
				glm::vec2(m_ViewportSize.x, m_ViewportSize.y));
			ASSInterface::Renderer::Submit(boxShader, vertexArrayBox, 2);*/

			ASSInterface::Renderer::EndScene();

			m_Framebuffer->Unbind();
		}
		
	}
	
}

void FrameVideo::OnEvent(ASSInterface::Event& event)
{
	if (m_ViewportFocused)
	{
		m_CameraController.OnEvent(event);
	}
	
}

void FrameVideo::OnImGuiRender()
{
		
}

void FrameVideo::ShowVideo(bool* p_open)
{
	ASS_PROFILE_FUNCTION();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	
	if (!ImGui::Begin(nameWindow.c_str(), p_open))
	{		
		ImGui::End();
		return;
	}

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	ASSInterface::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	{
		m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
	}
	
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();
}

void FrameVideo::RefreshBox()
{
	ASS_PROFILE_FUNCTION();

	float* coordinatesFace = m_Streamer->GetCoordinates();
	/*ASS_INFO("Left: {0}, Top: {1}, Width: {2}, Height: {3}", coordinatesFace[0], coordinatesFace[1], coordinatesFace[2], coordinatesFace[3]);*/
	float xc = (coordinatesFace[0] + (coordinatesFace[2] / 2.0f));
	float yc = (coordinatesFace[1] + (coordinatesFace[3] / 2.0f));

	float px = (xc / (float)m_Streamer->GetWidth()) * 100.0f; 
	float py = (yc / (float)m_Streamer->GetHeight()) * 100.0f; 

	x = (px * 2.0f) / 100;
	y = (py * 2.0f) / 100;

	if (x < 1.0f)
	{
		x = (1.0f - x) * -1.0f;
	}

	if (x >= 1.0f)
	{
		x = x - 1.0f;
	}
	
	if (y < 1.0f)
	{
		y = (1.0f - y);
	}

	if (y >= 1.0f)
	{
		y = (y - 1.0f) * -1.0f;
	}

	float prw = (coordinatesFace[2] / (float)m_Streamer->GetWidth()) * 100.0f;
	float prh = (coordinatesFace[3] / (float)m_Streamer->GetHeight()) * 100.0f;
	ratioWidth = prw / 100.0f; //
	ratioHeight = prh / 100.0f;

	/*if (check)
	{
		check = false;
		x = 0.0f, y = 0.0f, ratioHeight = 0.15f, ratioWidth = 0.2f;
	}
	else {
		check = true;
		x = -0.25f, y = 0.25f, ratioHeight = 0.1f, ratioWidth = 0.1f;
	}*/

	//ASS_INFO("X: {0}, Y: {1}, RW: {2}, RH: {3}", x, y, ratioWidth, ratioHeight);

	verticesBox.clear();
	verticesBox.push_back(x - ratioWidth); // BOTTOM LEFT TO BOTTOM RIGHT
	verticesBox.push_back(y - ratioHeight);
	verticesBox.push_back(x + ratioWidth);
	verticesBox.push_back(y - ratioHeight);

	verticesBox.push_back(x + ratioWidth); //BOTTOM RIGHT TO TOP RIGHT
	verticesBox.push_back(y - ratioHeight);
	verticesBox.push_back(x + ratioWidth);
	verticesBox.push_back(y + ratioHeight);

	verticesBox.push_back(x + ratioWidth); // TOP RIGHT TO TOP LEFT
	verticesBox.push_back(y + ratioHeight);
	verticesBox.push_back(x - ratioWidth);
	verticesBox.push_back(y + ratioHeight);

	verticesBox.push_back(x - ratioWidth); // TOP LEFT TO BOTTOM LEFT
	verticesBox.push_back(y + ratioHeight);
	verticesBox.push_back(x - ratioWidth);
	verticesBox.push_back(y - ratioHeight);

	ASSInterface::Ref<ASSInterface::VertexBuffer> vertexBufferBox;
	vertexBufferBox.reset(ASSInterface::VertexBuffer::Create(&verticesBox[0], (uint32_t)verticesBox.size() * sizeof(float))); //
	vertexBufferBox->SetLayout({
		{ ASSInterface::ShaderDataType::Float2, "a_Position" }
	});
	vertexArrayBox->PopVertexBuffer();
	vertexArrayBox->AddVertexBuffer(vertexBufferBox);

}

void FrameVideo::RunFlow()
{
	m_Streamer->Run();
}

void FrameVideo::InitTexture() {
	ASS_PROFILE_FUNCTION();

	vertexArrayTexture.reset(ASSInterface::VertexArray::Create());

	float squareVertices[5 * 4] = {
		 1.0f,  m_Streamer->GetSizeHeightTexture(), 0.0f, 1.0f, 1.0f,
		 1.0f, -m_Streamer->GetSizeHeightTexture(), 0.0f, 1.0f, 0.0f,
		-1.0f, -m_Streamer->GetSizeHeightTexture(), 0.0f, 0.0f, 0.0f,
		-1.0f,  m_Streamer->GetSizeHeightTexture(), 0.0f, 0.0f, 1.0f
	};

	ASSInterface::Ref<ASSInterface::VertexBuffer> squareVB;
	squareVB.reset(ASSInterface::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ASSInterface::ShaderDataType::Float3, "a_Position" },
		{ ASSInterface::ShaderDataType::Float2, "a_TexCoord" }
		});

	vertexArrayTexture->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 3, 1, 2, 3 };

	ASSInterface::Ref<ASSInterface::IndexBuffer> squareIB;
	squareIB.reset(ASSInterface::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	vertexArrayTexture->SetIndexBuffer(squareIB);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");		

	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

void FrameVideo::CreateBuffer()
{
	ASS_PROFILE_FUNCTION();
	std::vector<unsigned char> dataImage = m_Streamer->GetDataTexture();

	if (!dataImage.empty())
	{
		m_Texture = ASSInterface::Texture2D::Create(&dataImage[0], m_Streamer->GetWidth(),
			m_Streamer->GetHeight(), m_Streamer->GetChannels());
		ASSInterface::FramebufferSpecification fbSpec;
		fbSpec.Width = m_Streamer->GetWidth();
		fbSpec.Height = m_Streamer->GetHeight();
		m_Framebuffer = ASSInterface::Framebuffer::Create(fbSpec);
		m_CameraController.OnResize((float)m_Streamer->GetWidth(), (float)m_Streamer->GetHeight());
	}

}

void FrameVideo::StopThread(const std::string& tname) {
	flowRunning = false;	 
		
	if (m_Streamer->GetSourceFlow() == 3)
	{
		m_Streamer->Stop();
	}
	else
	{
		ThreadMap::const_iterator it = tm_.find(tname);
		if (it != tm_.end()) {
			it->second.std::thread::~thread(); // thread not killed
			tm_.erase(tname);
			m_Streamer->FinishLoop();
		}
			
	}
					
}

void FrameVideo::StartThread(const std::string& tname, bool runFlow) {
	flowRunning = runFlow;

	if (isBufferCreate && m_Streamer->GetSourceFlow() == 3)
	{
		m_Streamer->Play();
	}
	else
	{
		std::thread thrd = std::thread(&FrameVideo::RunFlow, this);
		thrd.detach();
		tm_[tname] = std::move(thrd);
		
	}
	
}