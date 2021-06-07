#include "hzpch.h"
#include "ASSInterface/Utils/PlatformUtils.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "ASSInterface/Core/Application.h"

namespace fs = std::filesystem;

namespace ASSInterface {	

	std::vector<std::string> FileDialogs::OpenFile(const char* filter)
	{
		std::vector<std::string> filePaths;
		OPENFILENAMEA ofn;
		CHAR szFile[2048] = { 0 }; //1024
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			CHAR* p = ofn.lpstrFile;
			std::string path = p;
			p += path.size() + 1;

			if (*p == 0)
			{
				// there is only one string, being the full path to the file
				filePaths.push_back(path.c_str());
			}
			else
			{
				// multiple files follow the directory
				for (; *p != 0; )
				{
					std::string fileName = p;
					filePaths.push_back((path + '\\' + fileName).c_str());
					p += fileName.size() + 1;
				}
			}
			
		}
		
		return filePaths;
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
	
}