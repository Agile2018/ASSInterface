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
			}

		}
		else
		{
			ASS_CORE_ERROR("Could not open file '{0}'", fileName);
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
}