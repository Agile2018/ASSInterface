#include "hzpch.h"
#include "Tracking.h"
#include "Platform/OpenCV/OpenCVTracking.h"
#include "Platform/AZUREFace/AZURETracking.h"

namespace ASSInterface {
	Ref<Tracking> Tracking::CreateOpenCV()
	{
		return CreateRef<OpenCVTracking>();
	}
	Ref<Tracking> Tracking::CreateAzureTrack()
	{
		return CreateRef<AZURETracking>();
	}
}