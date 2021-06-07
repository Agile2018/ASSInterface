#include "hzpch.h"
#include "GraphicProcessor.h"
#include "Platform/NVIDIA/NVIDIAProcessor.h"

namespace ASSInterface {
	Ref<GraphicProcessor> GraphicProcessor::WatchCUDA()
	{
		return CreateRef<NVIDIAProcessor>();
	}
}
