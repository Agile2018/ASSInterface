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
		virtual void SetPath(std::string nameFile) = 0;
		static Ref<Configuration> CreateConfigDatabase();
		static Ref<Configuration> CreateConfigVideo(std::string nameFile);
		static Ref<Configuration> CreateConfigGlobal();
		static Ref<Configuration> CreateConfigFace();
		static Ref<Configuration> CreateConfigTrack();
		static Ref<Configuration> CreateConfigEnroll();
		static Ref<Configuration> CreateConfigEntry();
		static Ref<Configuration> CreateConfigOnTheFly();
	private:		
		virtual void SaveFile(std::string content) = 0;			
	};
}
