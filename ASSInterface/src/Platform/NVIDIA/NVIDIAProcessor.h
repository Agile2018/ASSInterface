#pragma once

#include <ASSInterface.h>
#include <cuda_runtime.h>

namespace ASSInterface {
	class NVIDIAProcessor : public GraphicProcessor {
	public:
		NVIDIAProcessor();
		~NVIDIAProcessor();
		virtual inline std::string GetDescription() override { return description; };
		virtual bool ThereIsGraphicProcessor() override;
	private:
		std::string description;
	};
}
