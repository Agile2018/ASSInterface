#pragma once

#include <ASSInterface.h>
#include "iface.h"

namespace ASSInterface {
	class InnoErrorIFace : public ErrorRecognition {
	public:
		InnoErrorIFace();
		~InnoErrorIFace();
		virtual std::string GetError(int errorCode) override;
	private:
		std::string BuildMessageErrorLicense();
		std::string BuildMessageOtherError(int errorCode);
	};
}