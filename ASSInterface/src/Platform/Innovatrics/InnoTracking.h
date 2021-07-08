#pragma once

#include "ASSInterface.h"
#include "icao_status.h"

namespace ASSInterface {
	class InnoTracking : public Tracking {
	public:
		InnoTracking(int channel);
		~InnoTracking();
		virtual void Init() override;
		virtual void Detect(std::vector<unsigned char> data) override;
		virtual float* GetCoordinates() override;
		virtual float* GetColor() override;
		virtual inline const Rx::observable<DetectSpecification> Get() const { return observableSpecDetect; };
		virtual inline void SetWidth(int width) override { widthFrame = width; }
		virtual inline void SetHeight(int height) override { heightFrame = height; }
		virtual inline const std::vector<unsigned char> GetFlowData() const { return flowData; };
		virtual void Terminate() override;
		virtual inline void SetElapseFrame(int elapse) override {
			elapseFrame = elapse; 
			refreshDiscovery = discoveryFrecuence / elapseFrame;
		};
		virtual inline bool IsTrackFinish() override { return flagFinish; };
		virtual inline void SetTask(int value) override { task = value; };
		virtual inline int GetTask() override { return task; };
		virtual void ResetParams() override;
	private:
		virtual void CreateTemplate(void* face, DetectSpecification& specDetect) override;
		virtual void Crop(void* face, CropSpecification& specCrop) override;		
		void GetParametersTracking();
		void GetParamsDetectFace();
		void CheckError(std::string function, int errorCode);
		unsigned char* LoadImageFromMemory(std::vector<unsigned char> buffer,
			int* width, int* height);
		void ResetCoordinates();
		void ClearAllCoordinatesImage();
		void ClearCoordinatesImage(int indexTracked);
		void TrackObjectState();
		void SetCoordinatesImage(float x, float y, float width,
			float height, int indexTracked);
		void SetColorRectangle(float score, int indexTracked);
		void DoHomework(int indexTracked);
		void CreateFace(int indexTracked);
		void BuildSpecificationForIdentify(void* face);
		float ICAOFeature(void* face);
	private:				
		Rx::subject<DetectSpecification> specSubject;
		Rx::observable<DetectSpecification> observableSpecDetect = specSubject.get_observable();
		Rx::subscriber<DetectSpecification> shootSpecDetect = specSubject.get_subscriber();
		ASSInterface::Ref<ASSInterface::ErrorRecognition> errorIFace;
		ASSInterface::Ref<ASSInterface::Configuration> configurationTrack;
		ASSInterface::Ref<ASSInterface::Configuration> configurationFace;
		ASSInterface::Ref<ASSInterface::Detection> innoDetect = nullptr;
		int widthFrame = 0, heightFrame = 0, elapseFrame = 0;
		std::vector<unsigned char> flowData;
		int indexChannel, discoveryFrecuence, timeRedetect, tracking, 
			speedTrack, trackOptimus, maxDetected, confidenceThreshold, 
			speedDetect, speedExtraction, faceCrop, qualityModel, confidenceThresholdTrack, 
			detected, extraction, detectForced, multipleFace, refreshDiscovery = 0, residueCountFrames = 0;
		bool deepTracking;
		float minFace, maxFace;
		void* faceHandlerTracking = nullptr;
		void* objectHandler = nullptr;
		void* objects[NUM_TRACKED_OBJECTS] = {};
		long countFrameTracking = 0;
		bool flagFinish = false, isDataSend = true;
		float imageCoordinatesFollowed[COORDINATES_X_ALL_IMAGES] = {};
		float colorRectangle[NUM_TRACKED_OBJECTS] = {};
		int task = -1;
		bool isReset = false;
	};
}