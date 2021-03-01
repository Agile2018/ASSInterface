#pragma once

#include "hzpch.h"
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {
	class ASS_API Configuration
	{
	public:
		virtual ~Configuration() = default;		
		virtual void ParseToObject() = 0;
		virtual void ParseToFile() = 0;	
		virtual std::any GetParam(const char* name) = 0;
		virtual void SetParam(const char* name, std::any value) = 0;
		static Ref<Configuration> CreateConfigDatabase();
		static Ref<Configuration> CreateConfigVideo(std::string nameFile);
	private:		
		virtual void SaveFile(std::string content) = 0;			
	};
}
