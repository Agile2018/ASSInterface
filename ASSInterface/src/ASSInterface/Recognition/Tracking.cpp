#include "hzpch.h"
#include "Tracking.h"
#include "Platform/OpenCV/OpenCVTracking.h"

namespace ASSInterface {
	Ref<Tracking> Tracking::Create()
	{
		return CreateRef<OpenCVTracking>();
	}
}