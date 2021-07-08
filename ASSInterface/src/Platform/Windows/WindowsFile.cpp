#include "hzpch.h"
#include "ASSInterface.h"
#include "WindowsFile.h"

namespace ASSInterface {
	WindowsFile::WindowsFile()
	{
	}
	WindowsFile::~WindowsFile()
	{
	}
	std::string WindowsFile::ReadFile(std::string fileName)
	{
		
		std::string result;
		std::ifstream in(fileName, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				
				ASS_CORE_ERROR("Could not read from file '{0}'", fileName);		
				std::string str = "Could not read from file " + fileName;
				const char* err = str.c_str();
				ASS_ERROR_PROFILE_SCOPE("WindowsFile::ReadFile", err);
			}

		}
		else
		{
			ASS_CORE_ERROR("Could not open file '{0}'", fileName);
			std::string str = "Could not read from file " + fileName;
			const char* err = str.c_str();
			ASS_ERROR_PROFILE_SCOPE("WindowsFile::ReadFile", err);
		}

		return result;
		
	}

	void WindowsFile::WriteFile(std::string pathFile, std::string content)
	{
		std::ofstream file_obj;
		file_obj.open(pathFile, std::ios::app);
		file_obj.write(content.c_str(), sizeof(char) * content.size());
		file_obj.close();
	}

	bool WindowsFile::DelFile(std::string fileName)
	{
		if (remove(fileName.c_str()) != 0) {
			return false;
		}
		else {
			return true;
		}
		
	}
	void WindowsFile::CreateFolder(std::string nameFolder)
	{
		int result = _mkdir((char*)&nameFolder);
	}
	bool WindowsFile::IsFileExists(std::string path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);	
		
	}

	bool WindowsFile::SaveConfig()
	{
		const std::filesystem::path source = "imgui.ini";
		const std::filesystem::path target = "imgui_.ini";

		if (IsFileExists("imgui.ini"))
		{
			return std::filesystem::copy_file(source, target);
		}
		return false;
		
	}

	bool WindowsFile::ResetConfig()
	{
		if (DelFile("imgui.ini"))
		{
			const std::filesystem::path target = "imgui.ini";
			const std::filesystem::path source = "imgui_.ini";
			if (IsFileExists("imgui_.ini")) {
				return std::filesystem::copy_file(source, target);
			}
			
		}
		return false;
	}


}