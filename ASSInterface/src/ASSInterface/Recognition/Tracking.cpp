#include "hzpch.h"
#include "Tracking.h"
#include "Platform/OpenCV/OpenCVTracking.h"
#include "Platform/Innovatrics/InnoTracking.h"

namespace ASSInterface {
	Ref<Tracking> Tracking::CreateInnovatrics(int channel)
	{
		return CreateRef<InnoTracking>(channel);
	}
	Ref<Tracking> Tracking::CreateOpenCV()
	{
		return CreateRef<OpenCVTracking>();
	}	
}