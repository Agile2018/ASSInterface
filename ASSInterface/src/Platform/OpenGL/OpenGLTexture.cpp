#include "hzpch.h"
#include "OpenGLTexture.h"

#include <SOIL2.h>
#include <glad/glad.h>

namespace ASSInterface {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		ASS_PROFILE_FUNCTION();

		int width, height, channels;
		
		unsigned char* data = nullptr;
		{
			ASS_PROFILE_SCOPE("SOIL_load_image - OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
			data = SOIL_load_image(path.c_str(), &width, &height, &channels, 0);
		}
		

		ASS_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		ASS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
		
	}

	OpenGLTexture2D::OpenGLTexture2D(void* data, uint32_t width, uint32_t height, uint32_t channel) :
		m_Width(width), m_Height(height)
	{
		ASS_PROFILE_FUNCTION();
		GLenum internalFormat = 0, dataFormat = 0;
		if (channel == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;
		ASS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, 
			dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		ASS_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		ASS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ASS_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ASS_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}