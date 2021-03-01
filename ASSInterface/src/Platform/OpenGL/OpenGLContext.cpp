#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace ASSInterface {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		ASS_CORE_ASSERT(windowHandle, "Window handle is Null");
	}
	void OpenGLContext::Init()
	{
		ASS_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASS_CORE_ASSERT(status, "Failed to initialize Glad!");
		ASS_CORE_INFO("OpenGl Info:");
		ASS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ASS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ASS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		ASS_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}