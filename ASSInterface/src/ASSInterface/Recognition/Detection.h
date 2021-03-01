#pragma once

#include "hzpch.h"
#include <string>

namespace ASSInterface {
	struct CropSpecification {
		int length, width, height;
		unsigned char* crop;

		CropSpecification() {
			length = 0;
			width = 0;
			height = 0;
			crop = nullptr;
		}
	};

	struct DetectSpecification {
		CropSpecification cropSpec;
		char** templates;
		int* sizeData;

		DetectSpecification() {
			templates = nullptr;
			sizeData = nullptr;			
		}

	};


	class Detection {
	public:
		virtual void AddFiles(const std::string& files) = 0;
		virtual const DetectSpecification& Get() const = 0;
	private:
		virtual void Detect() = 0;
		virtual void CreateTemplate() = 0;
		virtual void Crop() = 0;
		virtual const CropSpecification& GetCropSpecification() const = 0;
	};
}
