#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace ASSInterface {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ASSInterface::ShaderDataType::Float:    return GL_FLOAT;
		case ASSInterface::ShaderDataType::Float2:   return GL_FLOAT;
		case ASSInterface::ShaderDataType::Float3:   return GL_FLOAT;
		case ASSInterface::ShaderDataType::Float4:   return GL_FLOAT;
		case ASSInterface::ShaderDataType::Mat3:     return GL_FLOAT;
		case ASSInterface::ShaderDataType::Mat4:     return GL_FLOAT;
		case ASSInterface::ShaderDataType::Int:      return GL_INT;
		case ASSInterface::ShaderDataType::Int2:     return GL_INT;
		case ASSInterface::ShaderDataType::Int3:     return GL_INT;
		case ASSInterface::ShaderDataType::Int4:     return GL_INT;
		case ASSInterface::ShaderDataType::Bool:     return GL_BOOL;
		}

		ASS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ASS_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ASS_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ASS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		ASS_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		ASS_PROFILE_FUNCTION();

		ASS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		ASS_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::PopVertexBuffer()
	{
		if (!m_VertexBuffers.empty())
		{
			m_VertexBuffers.pop_back();
		}
	}

}