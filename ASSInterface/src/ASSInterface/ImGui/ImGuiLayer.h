#pragma once

#include "ASSInterface/Core/Layer.h"
#include "ASSInterface/Events/ApplicationEvent.h"
#include "ASSInterface/Events/KeyEvent.h"
#include "ASSInterface/Events/MouseEvent.h"

namespace ASSInterface {

	class ASS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}