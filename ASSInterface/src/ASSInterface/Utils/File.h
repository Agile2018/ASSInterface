#pragma once
#include "hzpch.h"

namespace ASSInterface {
	class File
	{
	public:
		virtual std::string ReadFile(std::string fileName) = 0;
		virtual void WriteFile(std::string pathFile, std::string content) = 0;
		virtual bool DelFile(std::string fileName) = 0;
		virtual void CreateFolder(std::string nameFolder) = 0;
		virtual const std::string GetFolderConfiguration() const = 0;
		virtual bool IsFileExists(std::string path) = 0;
		virtual bool SaveConfig() = 0;
		virtual bool ResetConfig() = 0;
		static Ref<File> Create();
	};
}