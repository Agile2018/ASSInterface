#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

namespace ASSInterface {	

	class ErrorInstrumentor {
	private:		
		std::ofstream m_OutputStream;
		std::string filepath = "ASSInterfaceProfile-Error.json";
	private:
		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceErrors\":[{}";
			m_OutputStream.flush();
		}
		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

	public:
		ErrorInstrumentor() {};
		void BeginSession() {
			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open()) {				
				WriteHeader();
			}
		}

		void WriteProfile(std::string name, const char* description)
		{
			std::stringstream json;
			char buff[20];
			time_t now = time(NULL);
			strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			std::string timeError(buff);			
			std::string desc(description);
			std::replace(name.begin(), name.end(), '"', '\'');
			std::replace(desc.begin(), desc.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"date\":\"" << timeError << "\",";
			json << "\"name\":\"" << name << "\",";
			json << "\"description\":\"" << desc << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << std::this_thread::get_id();
			json << "}";
			
			m_OutputStream << json.str();
			m_OutputStream.flush();
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
		}

		static ErrorInstrumentor& Get() {
			static ErrorInstrumentor instance;
			return instance;
		}
	};
}

#define ASS_ERROR_PROFILE 1

#if ASS_ERROR_PROFILE
	#define ASS_ERROR_PROFILE_BEGIN_SESSION() ::ASSInterface::ErrorInstrumentor::Get().BeginSession()
	#define ASS_ERROR_PROFILE_END_SESSION() ::ASSInterface::ErrorInstrumentor::Get().EndSession()
	#define ASS_ERROR_PROFILE_SCOPE(name, description) ::ASSInterface::ErrorInstrumentor::Get().WriteProfile(name, description)
	
#else
	#define ASS_ERROR_PROFILE_BEGIN_SESSION()
	#define ASS_ERROR_PROFILE_END_SESSION() 
	#define ASS_ERROR_PROFILE_SCOPE(name, description) 

#endif