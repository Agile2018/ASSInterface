#include "hzpch.h"
#include "InnoErrorIDkit.h"

namespace ASSInterface {
	InnoErrorIDkit::InnoErrorIDkit()
	{
	}
	InnoErrorIDkit::~InnoErrorIDkit()
	{
	}
	std::string InnoErrorIDkit::GetError(int errorCode)
	{
		std::string msgResult;		
		char msg[256];

		sprintf_s(msg, "Error occurs! %s (code: %d)\n", 
			IEngine_GetErrorMsg(errorCode), errorCode);
		msgResult = msg;
		return msgResult;
	}
}