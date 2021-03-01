#pragma once

#include <string>

namespace ASSInterface {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::vector<std::string> OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		
	};

}
