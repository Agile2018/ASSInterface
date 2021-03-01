#pragma once
#include "hzpch.h"
#include "ASSInterface/Utils/File.h"
#include <direct.h>

namespace ASSInterface {
	class WindowsFile : public File
	{
	public:
		WindowsFile();
		~WindowsFile();
		virtual std::string ReadFile(std::string fileName) override;
		virtual void WriteFile(std::string pathFile, std::string content) override;
		virtual bool DelFile(std::string fileName) override;
		virtual void CreateFolder(std::string nameFolder) override;
		virtual const std::string GetFolderConfiguration() const override { return folderConfiguration; };
	private:
		const std::string folderConfiguration = "Configuration";
	};
}