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
		std::vector<char> faceSerialized;
		int sizeTemplate;
		int quality;
		int task;
		int id;
		int indexFace;
		float medianICAO = -1.0f;

		DetectSpecification() {			
			sizeTemplate = 0;
			quality = 0;
			task = -1;
			id = -1;
			indexFace = -1;
			float medianICAO = -1.0f;
		}

	};


	class Detection {
	public:
		virtual void BuildTemplatesFromPersonFiles(std::vector<std::string> files) = 0;
		virtual void BuildTemplatesFromPersonFiles(std::vector<std::string> files, int task, int id, int index = -1) = 0;
		virtual void Import(std::vector<std::string> files) = 0;
		virtual inline const Rx::observable<std::vector<DetectSpecification>> Get() const = 0;
		virtual inline const Rx::observable<concurrent_vector<DetectSpecification>> GetConcurrent() const = 0;
		virtual void ResetParameters() = 0;
		virtual std::vector<char> SerializeEntity(void* face) = 0;
		static Ref<Detection> CreateInnoDetected(int channel);
		static Ref<Detection> CreateInnoDetected();
	};
}
