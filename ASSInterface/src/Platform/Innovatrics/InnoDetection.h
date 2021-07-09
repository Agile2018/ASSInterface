#pragma once

#include "ASSInterface.h"
//#include "iface.h"
//#include "iface_common.h"
#include "icao_status.h"
#include "tbb/tbb.h"

using namespace tbb;

#pragma warning( disable: 588)

namespace ASSInterface {	
	class InnoDetection : public Detection {
	public:
		InnoDetection(int channel);
		InnoDetection();
		~InnoDetection();
		virtual void BuildTemplatesFromPersonFiles(std::vector<std::string> files) override;
		virtual void BuildTemplatesFromPersonFiles(std::vector<std::string> files, 
			int task, int id, int index = -1) override;

		virtual void Import(std::vector<std::string> files) override;
		virtual void ResetParameters() override;
		virtual std::vector<char> SerializeEntity(void* face) override;
		virtual inline const Rx::observable<std::vector<DetectSpecification>> Get() const override { return observableSpecDetected; };
		virtual inline const Rx::observable<concurrent_vector<DetectSpecification>> GetConcurrent() const override { return observableConcurrentSpecDetected; }
	private:
		std::vector<void*> Detect(std::vector<float>& icaoScore);
		void Crop(void* face, CropSpecification& specCrop);
		void SetParameters();		
		void ParallelApply(std::vector<std::string> files, size_t n);
		void GetParamsDetectFace();
		void CheckError(std::string function, int errorCode);
		unsigned char* LoadFileImage(std::string fileImage, int* width, 
			int* height, int* length);
		void LoadBufferImages(std::vector<std::string> files);
		unsigned char* LoadImageMemory(std::vector<unsigned char> buffer,
			int* width, int* height, int* lenght);
		std::vector<DetectSpecification> GetBatchTemplates(std::vector<void*> faces);
		std::vector<std::vector<int>> ValidateQuality(
			std::vector<DetectSpecification>& detectTemplates);
		std::vector<CropSpecification> CropValidateTemplates(
			std::vector<void*> faces, std::vector<std::vector<int>> validate);
		std::vector<char> SerializeEntity(std::vector<void*> faces, int index);		
		void SendValidDetection(std::vector<DetectSpecification> templatesGet,
			std::vector<std::vector<int>> valid, 
			std::vector<CropSpecification> cropValid, 
			int task, std::vector<char> faceSerialized, std::vector<float> icaoScore, int id = -1, int idx = -1);
		void BuildSpecificationForIdentify(int task, int id = -1, int index = -1);
		void BuildTemplateOfFile(std::string file);		
		
	private:		
		Rx::subject<std::vector<DetectSpecification>> specSubject;
		Rx::observable<std::vector<DetectSpecification>> observableSpecDetected = specSubject.get_observable();
		Rx::subscriber<std::vector<DetectSpecification>> shootSpecDetected = specSubject.get_subscriber();

		Rx::subject<concurrent_vector<DetectSpecification>> specConcurrentSubject;
		Rx::observable<concurrent_vector<DetectSpecification>> observableConcurrentSpecDetected = specConcurrentSubject.get_observable();
		Rx::subscriber<concurrent_vector<DetectSpecification>> shootSpecConcurrentDetected = specConcurrentSubject
			.get_subscriber();

		ASSInterface::Ref<ASSInterface::Configuration> configurationFace;
		ASSInterface::Ref<ASSInterface::ErrorRecognition> errorIFace;
		ASSInterface::Ref<ASSInterface::TransformImage> transImage;
		int indexChannel, confidenceThreshold, speedDetect, 
			speedExtraction, faceCrop, qualityModel, maxDetected;
		float minFace = 0.0f, maxFace = 0.0f;
		void* faceHandler = nullptr;
		bool isSetParameters = false;
		std::vector<std::vector<unsigned char>> bufferOfImages;
		std::vector<std::vector<int>> dimensionsImages;	
		/*concurrent_queue<DetectSpecification> importDetect;*/
		concurrent_vector<DetectSpecification> importDetect;
	};
}
