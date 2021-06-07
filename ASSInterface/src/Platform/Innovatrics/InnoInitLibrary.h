#pragma once

#include <ASSInterface.h>
#include "iface.h"
#include "idkit.h"

namespace ASSInterface {
	class InnoInitLibrary : public InitLibrary {
	public:
		InnoInitLibrary();
		~InnoInitLibrary();
		virtual void Init() override;
		virtual void Terminate() override;
	private:
		ASSInterface::Ref<ASSInterface::GraphicProcessor> watchProcessor;
		ASSInterface::Ref<ASSInterface::Configuration> configurationGlobals;
		ASSInterface::Ref<ASSInterface::ErrorRecognition> errorIFace;
		ASSInterface::Ref<ASSInterface::ErrorIdentification> errorIDkit;
		bool isGraphicProcessor = false;
		int idGPU,  enabledGPU, mode, threadNumber;
		std::string connectDatabase = "iengine.db";
	private:
		void SetParamsLibrary();
		void GetParametersGlobals();
		bool SetParamsGraphicProcessor();
		void InitIFACE();
		void InitIDKIT();
	};
}