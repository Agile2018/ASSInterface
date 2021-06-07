#include "hzpch.h"
#include "NVIDIAProcessor.h"

namespace ASSInterface {
	NVIDIAProcessor::NVIDIAProcessor()
	{
	}
	NVIDIAProcessor::~NVIDIAProcessor()
	{
	}
	bool NVIDIAProcessor::ThereIsGraphicProcessor()
	{
		description = "-----------------------------------------\n";
		description += "CUDA HARDWARE REPORT\n";

		int deviceCount = 0;
		cudaError_t error_id = cudaGetDeviceCount(&deviceCount);

		if (error_id != cudaSuccess) {

			std::string errorDesc = cudaGetErrorString(error_id);

			description += "cudaGetDeviceCount returned " + std::to_string(static_cast<int>(error_id)) + "\n";
			description += "->" + errorDesc + "\n";
			description += "-----------------------------------------\n";
			return false;
		}

		// This function call returns 0 if there are no CUDA capable devices.
		if (deviceCount == 0) {
			description += "There are no available device(s) that support CUDA\n";
			description += "-----------------------------------------\n";
			return false;
		}
		else {
			description += "Detected " + std::to_string(deviceCount) + " CUDA Capable device(s) OK\n";
			description += "-----------------------------------------\n";
			return true;
		}
		
	}
}