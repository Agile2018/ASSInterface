#pragma once

#include <string>
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {
	class ErrorRecognition {
	public:
		virtual std::string GetError(int errorCode) = 0;
		static Ref<ErrorRecognition> CreateIFace();
	};
}