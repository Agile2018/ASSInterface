#include "hzpch.h"
#include "Detection.h"
#include "Platform/Innovatrics/InnoDetection.h"

namespace ASSInterface {
	Ref<Detection> Detection::CreateInnoDetected(int channel)
	{
		return CreateRef<InnoDetection>(channel);
	}
}