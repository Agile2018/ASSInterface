#pragma once

#include "ASSInterface.h"
#include "tbb/tbb.h"

using namespace tbb;

#pragma warning( disable: 588)


namespace ASSInterface {
	class InnoTask : public ExecuteTask {
	public:
		InnoTask(int channel);
		~InnoTask();		
		virtual inline const Rx::observable<IdentitySpecification> Get() const override { 
			return observableSpecIdentify; }		
		virtual inline const Rx::observable<int> EndTask() const override { return observableEnd; };
		virtual inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) override { dbMongo = db; }
		virtual void CloseConnection() override;
		virtual void ResetParameters() override;
		virtual void DoTask(DetectSpecification specDetect) override;
		virtual void DoTask(concurrent_vector<DetectSpecification> concurrentDetection) override;
		virtual void DoTask(std::vector<DetectSpecification> specDetects) override;
		virtual void Enroll(std::vector<char> templateFace, int* id) override;
		virtual void EnrollTemplates(std::vector<TemplateSpecification> specTemplate, 
			int* id) override;
		virtual void Remove(int id) override;
		virtual void Import(concurrent_vector<DetectSpecification> concurrentDetection) override;
	private:		
		void GetParamsEnroll();
		void ExecuteControlEntry(DetectSpecification specDetect);
		void ExecuteControlEntryTrack(DetectSpecification specDetect);
		bool CheckUnidentified(const unsigned char* data, int size);
		void SendUnidentified(DetectSpecification& specDetect);
		void SendIdentified(DetectSpecification& specDetect, int id, int score);	
		void EnrollPersonAndTemplates(std::vector<DetectSpecification> specDetects);
		bool EnrollmentIsExecuted(const unsigned char* data, int size);
		void SendEnrolled(DetectSpecification& specDetect, int id, int numberTemplates);
		void BuildForEnrollment(DetectSpecification specDetect);		
		void EnrollImportDatabase(DetectSpecification specDetect, int id);
		void EnrollImport(DetectSpecification specDetect);
	private:
		Rx::subject<IdentitySpecification> specSubject;
		Rx::observable<IdentitySpecification> observableSpecIdentify = specSubject.get_observable();
		Rx::subscriber<IdentitySpecification> shootSpecIdentify = specSubject.get_subscriber();		
	
		Rx::subject<int> endSubject;
		Rx::observable<int> observableEnd= endSubject.get_observable();
		Rx::subscriber<int> shootEnd = endSubject.get_subscriber();


		ASSInterface::Ref<ASSInterface::Identification> innoIdentify;
		ASSInterface::Ref<ASSInterface::Configuration> configurationEnroll;
		ASSInterface::Ref<ASSInterface::Database> dbMongo;
		ASSInterface::Ref<ASSInterface::Base64> base64;		

		int indexChannel, gallery, deduplicate, concatenate, 
			concatenateMode, minScore, maxScore, maxTemplates;
		bool isConnect = false;
		std::vector<unsigned char> unidentified;				
		int countEnrolled = 0;
	};
}