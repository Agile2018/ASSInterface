#pragma once

#include "hzpch.h"
#include "Detection.h"

namespace ASSInterface {
	class Tracking {
	public:
		virtual void Init() = 0;
		virtual void Detect(std::vector<unsigned char> data) = 0;
		virtual float** GetCoordinates() = 0;
		virtual float* GetColor() = 0;
		virtual const DetectSpecification& Get() const = 0;
		virtual inline void SetWidth(int width) = 0;
		virtual inline void SetHeight(int height) = 0;
		virtual inline const std::vector<unsigned char> GetFlowData() const = 0;
		static Ref<Tracking> Create();
	private:
		virtual void CreateTemplate() = 0;
		virtual void Crop() = 0;
		virtual const CropSpecification& GetCropSpecification() const = 0;
	};
}

