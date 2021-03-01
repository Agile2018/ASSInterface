#include "hzpch.h"
#include "ASSInterface/Renderer/GraphicsContext.h"

#include "ASSInterface/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace ASSInterface {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    ASS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		ASS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}