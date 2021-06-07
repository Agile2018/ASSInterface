#include "hzpch.h"
#include "FrameVideo.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2.h>

FrameVideo::FrameVideo() : Layer("FrameVideo"), m_CameraController(1024.0f / 768.0f) //  1280.0f / 720.0f  1280.0f / 960.0f
{
	ASS_PROFILE_FUNCTION();

	transImage = ASSInterface::TransformImage::Create();
	base64 = ASSInterface::Base64::Create();
	float x = 0.0f;
	float y = 0.0f;
	float ratioWidth = 0.0f;
	float ratioHeight = 0.0f;

	indicesBox.push_back(0);
	indicesBox.push_back(1);
	indicesBox.push_back(2);
	indicesBox.push_back(3);
	indicesBox.push_back(4);
	indicesBox.push_back(5);
	indicesBox.push_back(6);
	indicesBox.push_back(7);	

	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
	{
		ASSInterface::Ref<ASSInterface::VertexArray> vertexArrayBox;
		vertexArrayBox.reset(ASSInterface::VertexArray::Create());			
		vertexArrayBoxes.push_back(vertexArrayBox);
	}
	std::vector<float> verticesBox;
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

	indexBufferBox.reset(ASSInterface::IndexBuffer::Create(&indicesBox[0], sizeof(indicesBox) / sizeof(uint32_t)));
	
	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
		vertexArrayBoxes[i]->AddVertexBuffer(vertexBufferBox);
		vertexArrayBoxes[i]->SetIndexBuffer(indexBufferBox);
	}


	shaderBox = m_ShaderLibrary.Load("assets/shaders/BoxShader.glsl"); //RectangleShader

	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->Bind();
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniformFloat4("u_Color", { 0.2f, 0.2f, 0.9f, 1.0f });
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniformFloat("u_factor", factor);
	std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(shaderBox)->UploadUniform1ui("u_pattern", pattern);
	
}

void FrameVideo::Init(int channel) {
	ASS_PROFILE_FUNCTION();	
	m_Streamer = ASSInterface::ASSStreamer::Create(channel);
	if (m_Streamer->IsCorrectDescription())
	{
		nameWindow = m_Streamer->GetNameWindow();
		StartThread(threadInit, false);

	}
	else
	{
		*messageStatus = "File video incorrect or not exist.";
	}
}

FrameVideo::~FrameVideo()
{
	
	indicesBox.clear();
	m_Streamer->FinishLoop();
	vertexArrayTexture->Unbind();
	for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
		vertexArrayBoxes[i]->Unbind();
	}

	vertexArrayBoxes.clear();
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

	/*if (ASSInterface::Input::IsKeyPressed(ASS_KEY_F5))
	{
		
	}*/

	if (isBufferCreate && flowRunning)
	{
		
		if (ASSInterface::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
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
		m_Streamer->InitTracking();
		StopThread(threadInit);
		ObserverIdentify();
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
				RefreshBoxes();				
				m_Texture->SetData(&data[0], m_Streamer->GetSizeImage());
			}
			data.clear();
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
			textureShader->Unbind();
			textureShader = nullptr;

			auto boxShader = m_ShaderLibrary.Get("BoxShader"); //RectangleShader

			for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {				
				shaderBox->Bind();				
				std::dynamic_pointer_cast<ASSInterface::OpenGLShader>(boxShader)->UploadUniformFloat2("u_resolution", glm::vec2(m_ViewportSize.x, m_ViewportSize.y));
				ASSInterface::Renderer::Submit(boxShader, vertexArrayBoxes[i], 2);
				shaderBox->Unbind();

			}
			boxShader->Unbind();
			boxShader = nullptr;

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
	
	if (!ImGui::Begin(nameWindow.c_str(), p_open))
	{		
		ImGui::End();
		return;
	}

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	ASSInterface::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

	ImVec2 viewportPanelSize = ImGui::GetContentRegionMax(); // () GetContentRegionAvail

	if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	{
		
		m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y); 
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y }; 

		m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y); 
	}
	
	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, 
		ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	
	ImGui::PopStyleVar();
	ImGui::End();
	
}

ASSInterface::Ref<ASSInterface::ExecuteTask> FrameVideo::GetExecuteTask()
{
	std::any anyInnoTask = m_Streamer->GetExecuteTask();

	ASSInterface::Ref<ASSInterface::ExecuteTask> exeTask = std::any_cast<ASSInterface::Ref<ASSInterface::ExecuteTask>>(anyInnoTask);
	return exeTask;
}

void FrameVideo::RefreshBoxes()
{
	ASS_PROFILE_FUNCTION();

	float* coordinatesFace = m_Streamer->GetCoordinates();

	for (int i = 0; i < COORDINATES_X_ALL_IMAGES; i += 4) {
		float x = coordinatesFace[i];
		float y = coordinatesFace[i + 1];
		float ratioWidth = coordinatesFace[i + 2];
		float ratioHeight = coordinatesFace[i + 3];

		std::vector<float> verticesBox;

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
		vertexBufferBox.reset(ASSInterface::VertexBuffer::Create(&verticesBox[0],
			(uint32_t)verticesBox.size() * sizeof(float)));
		vertexBufferBox->SetLayout({
			{ ASSInterface::ShaderDataType::Float2, "a_Position" }
		});

		int index = i / 4;
		vertexArrayBoxes[index]->PopVertexBuffer();
		vertexArrayBoxes[index]->AddVertexBuffer(vertexBufferBox);
		verticesBox.clear();
	}
	coordinatesFace = nullptr;
}

void FrameVideo::RunFlow()
{

	m_Streamer->Run();
	if (!m_Streamer->IsCorrectStart()) {
		StopThread(threadInit);
	}
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

	/*float squareVertices[5 * 4] = {
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};*/

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

void FrameVideo::ObserverIdentify()
{
	auto identifyObservable = m_Streamer->GetIdentify()
		.map([](ASSInterface::IdentitySpecification specIdentify) {
			return specIdentify;
		});

	auto subscriptionIdentify = identifyObservable
		.subscribe([this](ASSInterface::IdentitySpecification specIdentify) {		
			SetSpecificationPerson(specIdentify);					
		});

	subscriptionIdentify.clear();
}

void FrameVideo::SetSpecificationPerson(ASSInterface::IdentitySpecification& specIdentify)
{
	
	std::string currentDate = ASSInterface::DateTime::Now();
	PersonSpecification personSpecification;	

	personSpecification.bufferCapture.assign(&specIdentify.cropData[0], 
		&specIdentify.cropData[0] + specIdentify.cropLength);
	personSpecification.cropWidth = specIdentify.cropWidth;
	personSpecification.cropHeight = specIdentify.cropHeight;
	personSpecification.task = specIdentify.task;
	strcpy(personSpecification.id, std::to_string(specIdentify.id).c_str());
	strcpy(personSpecification.name, specIdentify.name.c_str());
	strcpy(personSpecification.lastname, specIdentify.lastName.c_str());
	strcpy(personSpecification.document, specIdentify.document.c_str());	
	strcpy(personSpecification.match, std::to_string(specIdentify.match).c_str());
	strcpy(personSpecification.channel, std::to_string(specIdentify.channel).c_str());
	strcpy(personSpecification.type, specIdentify.gallery.c_str());	
	strcpy(personSpecification.date, currentDate.c_str());

	if (!specIdentify.imageDB.empty())
	{
		std::string decImg = base64->base64_decode(specIdentify.imageDB);		
		std::vector<char> imgStr(decImg.c_str(), decImg.c_str() + decImg.size() + 1);
		unsigned char* imgDB = reinterpret_cast<unsigned char*>(&imgStr[0]);	
		personSpecification.bufferGallery.assign(imgDB, imgDB + imgStr.size());

	}
	
	if (!specIdentify.docObverse.empty())
	{
		std::string decDoc = base64->base64_decode(specIdentify.docObverse);
		std::vector<char> imgStr(decDoc.c_str(), decDoc.c_str() + decDoc.size() + 1);
		unsigned char* imgDoc = reinterpret_cast<unsigned char*>(&imgStr[0]);
		personSpecification.bufferDocumentObverse.assign(imgDoc, imgDoc + imgStr.size());

	}

	if (!specIdentify.docReverse.empty())
	{
		std::string decDoc = base64->base64_decode(specIdentify.docReverse);
		std::vector<char> imgStr(decDoc.c_str(), decDoc.c_str() + decDoc.size() + 1);
		unsigned char* imgDoc = reinterpret_cast<unsigned char*>(&imgStr[0]);
		personSpecification.bufferDocumentReverse.assign(imgDoc, imgDoc + imgStr.size());

	}


	int sizeTemplate = specIdentify.size;
	if (sizeTemplate != 0)
	{
		personSpecification.size = sizeTemplate;
		personSpecification.templateFace.assign(&specIdentify.templateData[0],
			&specIdentify.templateData[0] + sizeTemplate);		
	}
		
	shootSpecPerson.on_next(personSpecification);
	
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
	if (m_Streamer->IsCorrectStart()) {
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
		m_Streamer->SetTask(-1);
	}
	else
	{
		ThreadMap::const_iterator it = tm_.find(tname);
		if (it != tm_.end()) {
			it->second.std::thread::~thread(); 
			tm_.erase(tname);			
		}

		*messageStatus = "File video incorrect or not exist.";
	}
					
}

void FrameVideo::StartThread(const std::string& tname, bool runFlow) {
	if (m_Streamer->IsCorrectStart() || !runFlow) {
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
	else
	{
		*messageStatus = "File video incorrect or not exist.";
	}
	
}