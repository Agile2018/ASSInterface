#include "hzpch.h"
#include "Stream.h"
#include "Platform/GStreamer/ASGStreamer.h"

namespace ASSInterface {
	Ref<ASSStreamer> ASSStreamer::Create(int channel)
	{
		return CreateRef<ASGStreamer>(channel);
	}
}