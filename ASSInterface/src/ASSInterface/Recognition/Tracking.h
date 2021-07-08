#pragma once

#include "hzpch.h"
#include "Detection.h"

namespace ASSInterface {
	class Tracking {
	public:
		virtual void Init() = 0;
		virtual void Detect(std::vector<unsigned char> data) = 0;
		virtual float* GetCoordinates() = 0;
		virtual float* GetColor() = 0;
		virtual inline const Rx::observable<DetectSpecification> Get() const = 0;
		virtual inline void SetWidth(int width) = 0;
		virtual inline void SetHeight(int height) = 0;
		virtual inline const std::vector<unsigned char> GetFlowData() const = 0;	
		virtual void Terminate() = 0;
		virtual inline void SetElapseFrame(int elapse) = 0;
		virtual inline bool IsTrackFinish() = 0;
		virtual inline void SetTask(int value) = 0;		
		virtual inline int GetTask() = 0;
		virtual void ResetParams() = 0;		
		static Ref<Tracking> CreateInnovatrics(int channel);
		static Ref<Tracking> CreateOpenCV();		
	private:
		virtual void CreateTemplate(void* face, DetectSpecification& specDetect) = 0;
		virtual void Crop(void* face, CropSpecification& specCrop) = 0;		
	};
}

