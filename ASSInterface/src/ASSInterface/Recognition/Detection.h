#pragma once

#include "hzpch.h"
#include <string>
#include "tbb/tbb.h"

using namespace tbb;

#pragma warning( disable: 588)

namespace ASSInterface {
	struct CropSpecification {
		int length, width, height;
		std::vector<unsigned char> cropData;

		CropSpecification() {
			length = 0;		
			width = 0;
			height = 0;			
		}
	};

	struct DetectSpecification {
		CropSpecification cropSpec;
		std::vector<char> templateData;
		int sizeTemplate;
		int quality;
		int task;

		DetectSpecification() {			
			sizeTemplate = 0;
			quality = 0;
			task = -1;
		}

	};


	class Detection {
	public:
		virtual void BuildTemplatesFromPersonFiles(std::vector<std::string> files) = 0;
		virtual void Import(std::vector<std::string> files) = 0;
		virtual inline const Rx::observable<std::vector<DetectSpecification>> Get() const = 0;
		virtual inline const Rx::observable<concurrent_vector<DetectSpecification>> GetConcurrent() const = 0;
		virtual void ResetParameters() = 0;
		static Ref<Detection> CreateInnoDetected(int channel);
			
	};
}
