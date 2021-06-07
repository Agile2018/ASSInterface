#include "hzpch.h"
#include "InnoErrorIFace.h"

namespace ASSInterface {
	InnoErrorIFace::InnoErrorIFace()
	{
	}
	InnoErrorIFace::~InnoErrorIFace()
	{
	}
	std::string InnoErrorIFace::GetError(int errorCode)
	{
		std::string msg;

		if (errorCode == IFACE_ERR_LICENSE_INTEGRATION_GENERIC)
		{
			msg = BuildMessageErrorLicense();
		}
		else
		{
			msg = BuildMessageOtherError(errorCode);
		}

		return msg;
	}
	std::string InnoErrorIFace::BuildMessageErrorLicense()
	{
		std::string msgResult;
		int hwIdLen = 1024;
		char hwId[1024];

		IFACE_GetHardwareId(hwId, &hwIdLen);
	
		char msg[256];
		sprintf_s(msg, "Your license is invalid or not present, please contact support for license with this HwId %s", hwId);
		msgResult = msg;

		return msgResult;
	}

	std::string InnoErrorIFace::BuildMessageOtherError(int errorCode)
	{
		std::string msgResult;
		const int errorBufferLength = 1024;
		char errorBuffer[errorBufferLength];
		char msg[256];

		int getErrorMsgErrorCode = IFACE_GetErrorMessage(errorCode,
			errorBufferLength, errorBuffer);

		if (getErrorMsgErrorCode == IFACE_OK)
		{
			sprintf_s(msg, "Error occurs! %s (code: %d)", errorBuffer, errorCode);
			msgResult = msg;
		}
		else
		{
			sprintf_s(msg, "Error occurs! %s (code %d)", "Error occurred during error code to message translation", getErrorMsgErrorCode);
			msgResult = msg;
		}

		return msgResult;
	}
}