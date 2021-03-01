#pragma once

#include "Core.h"
#include "ASSInterface/Events/Event.h"
#include "Window.h"
#include "ASSInterface/Core/LayerStack.h"
#include "ASSInterface/Events/ApplicationEvent.h"
#include "ASSInterface/ImGui/ImGuiLayer.h"
#include "ASSInterface/Core/Timestep.h"

namespace ASSInterface {

	class ASS_API Application
	{
	public:
		Application(const std::string& name = "ASSInterface App");
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		inline Window& GetWindow() { return *m_Window; }
		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
			
	private:
		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();
}



