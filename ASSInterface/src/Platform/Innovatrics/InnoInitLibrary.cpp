#include "hzpch.h"
#include "InnoInitLibrary.h"

namespace ASSInterface {
	InnoInitLibrary::InnoInitLibrary()
	{
		errorIFace = ASSInterface::ErrorRecognition::CreateIFace();
		errorIDkit = ASSInterface::ErrorIdentification::CreateIDkit();
		watchProcessor = ASSInterface::GraphicProcessor::WatchCUDA();
		isGraphicProcessor = watchProcessor->ThereIsGraphicProcessor();
		ASS_INFO("Graphic Processor: {0}", watchProcessor->GetDescription());

		GetParametersGlobals();
	}
	InnoInitLibrary::~InnoInitLibrary()
	{
		
	}
	void InnoInitLibrary::Init()
	{		
		SetParamsLibrary();
		InitIFACE();
		InitIDKIT();

	}
	void InnoInitLibrary::SetParamsLibrary()
	{
		int errorCode = -1;
		if (isGraphicProcessor && enabledGPU == 1) {
			if (SetParamsGraphicProcessor())
			{
				ASS_INFO("CUDA SET INNOVATRICS OK.");
				
			}
		}

		switch (mode)
		{
		case 0:
			errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
				IFACE_PARAMETER_GLOBAL_THREAD_MANAGEMENT_MODE,
				IFACE_GLOBAL_THREAD_MANAGEMENT_MODE_MAX_PARALLEL);
			break;
		case 1:
			errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
				IFACE_PARAMETER_GLOBAL_THREAD_MANAGEMENT_MODE,
				IFACE_GLOBAL_THREAD_MANAGEMENT_MODE_MIN_MEMORY);
			break;
		case 2:
			errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
				IFACE_PARAMETER_GLOBAL_THREAD_MANAGEMENT_MODE,
				IFACE_GLOBAL_THREAD_MANAGEMENT_MODE_SINGLE);
			break;
		default:
			break;
		}

		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::SetParamsLibrary", errorIFace->GetError(errorCode).c_str());
		}

		errorCode = IFACE_SetParam(IFACE_GLOBAL_PARAMETERS,
			IFACE_PARAMETER_GLOBAL_THREAD_NUM, std::to_string(threadNumber).c_str()); //IFACE_GLOBAL_THREAD_NUM_DEFAULT  "2"
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::SetParamsLibrary", errorIFace->GetError(errorCode).c_str());
		}		
				
	}
	void InnoInitLibrary::Terminate()
	{
		int errorCode;

		errorCode = IFACE_Terminate();
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::Terminate", errorIFace->GetError(errorCode).c_str());
		}
		
		errorCode = IEngine_TerminateModule();
		if (errorCode != IENGINE_E_NOERROR) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::Terminate", 
				errorIDkit->GetError(errorCode).c_str());
		}
	}

	bool InnoInitLibrary::SetParamsGraphicProcessor()
	{
		int errorCode;
		errorCode = IFACE_SetParam(nullptr,
			IFACE_PARAMETER_GLOBAL_GPU_ENABLED, (enabledGPU == 0) ? "false" : "true");
		if (errorCode == IFACE_OK) {
			errorCode = IFACE_SetParam(nullptr,
				IFACE_PARAMETER_GLOBAL_GPU_DEVICE_ID, std::to_string(idGPU).c_str());
			if (errorCode != IFACE_OK) {
				ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::SetParamsGraphicProcessor", 
					errorIFace->GetError(errorCode).c_str());			
				return false;
			}
		}
		else {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::SetParamsGraphicProcessor",
				errorIFace->GetError(errorCode).c_str());
			return false;
		}
		return true;
	}
	void InnoInitLibrary::GetParametersGlobals()
	{
		configurationGlobals = ASSInterface::Configuration::CreateConfigGlobal();
		configurationGlobals->ParseToObject();

		std::any anyGpu = configurationGlobals->GetParam("GLOBAL_GPU_ENABLED");
		std::any anyId = configurationGlobals->GetParam("GLOBAL_GPU_DEVICE_ID");
		std::any anyMode = configurationGlobals->GetParam("GLOBAL_THREAD_MANAGEMENT_MODE");
		std::any anyNum = configurationGlobals->GetParam("GLOBAL_THREAD_NUM");		

		idGPU = std::any_cast<int>(anyId);
		enabledGPU = std::any_cast<int>(anyGpu);
		mode = std::any_cast<int>(anyMode);
		threadNumber = std::any_cast<int>(anyNum);
	
	}
	void InnoInitLibrary::InitIFACE()
	{
		int errorCode;

		errorCode = IFACE_Init();
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::Init", 
				errorIFace->GetError(errorCode).c_str());
		}
		else
		{
			ASS_INFO("IFACE INNOVATRICS OK.");
		}
	}
	void InnoInitLibrary::InitIDKIT()
	{
		int errorCode, userLimit;
		const char* vers_string = IEngine_GetProductString();
		ASS_INFO("Product string is: {0}", vers_string);
		errorCode = IEngine_InitModule();
		if (errorCode != IENGINE_E_NOERROR) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::InitIDKIT",
				errorIDkit->GetError(errorCode).c_str());
		}
		else
		{
			ASS_INFO("IDKIT INNOVATRICS OK.");
		}
		errorCode = IEngine_GetUserLimit(&userLimit);
		ASS_INFO("Maximum permitted number of users in the database: {0}", userLimit);
		errorCode = IEngine_SetParameter(CFG_LOG_LEVEL, 0);
		errorCode = IEngine_InitClient(connectDatabase.c_str());
		if (errorCode != IENGINE_E_NOERROR) {
			ASS_ERROR_PROFILE_SCOPE("InnoInitLibrary::InitIDKIT",
				errorIDkit->GetError(errorCode).c_str());
		}
		
	}
}