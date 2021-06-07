#pragma once

#include "hzpch.h"
#include <string>
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {
	class ASS_API GraphicProcessor {
	public:
		virtual inline std::string GetDescription() = 0;
		virtual bool ThereIsGraphicProcessor() = 0;
		static Ref<GraphicProcessor> WatchCUDA();
	};
}