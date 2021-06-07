#include "hzpch.h"
#include "ErrorRecognition.h"
#include "Platform/Innovatrics/InnoErrorIFace.h"

namespace ASSInterface {
	Ref<ErrorRecognition> ErrorRecognition::CreateIFace()
	{
		return CreateRef<InnoErrorIFace>();
	}
}