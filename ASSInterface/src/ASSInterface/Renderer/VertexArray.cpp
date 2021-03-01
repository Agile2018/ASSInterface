#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ASSInterface {

	VertexArray* VertexArray::Create()
	{		

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    ASS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		ASS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}