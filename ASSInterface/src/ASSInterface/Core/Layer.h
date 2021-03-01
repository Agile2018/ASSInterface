#pragma once

#include "Core.h"
#include "ASSInterface/Core/Timestep.h"
#include "ASSInterface/Events/Event.h"

namespace ASSInterface {

	class ASS_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");	
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {} 
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName = "Layer";
	};

}

