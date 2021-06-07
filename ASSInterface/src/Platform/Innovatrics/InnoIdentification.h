#pragma once

#include "ASSInterface.h"
#include "idkit.h"

namespace ASSInterface {
	class InnoIdentification : public Identification {
	public:
		InnoIdentification(int channel);
		~InnoIdentification();
		virtual void ResetParameters() override;
		virtual void SetParameters(int option) override;
		virtual void LoadConnection(int option) override;
		virtual void CloseConnection() override;
		virtual void Enroll(const unsigned char* tpt,
			int size, int* id) override;
		virtual void AddTemplate(const unsigned char* tpt, int size) override;
		virtual void AddTemplate(const unsigned char* tpt,
			int size, int id) override;
		virtual unsigned char* GetTemplate(int id, int* lenght, int index) override;
		virtual void EnrollWithData(const unsigned char* tpt, int size,
			const unsigned char* data, int dataLength, int* id) override;
		virtual void ClearEnroll() override;
		virtual void Remove(int id) override;
		virtual void Find(const unsigned char* tpt, int size, int* id, int* score) override;
		virtual void Match(const unsigned char* tpt1, int size1,
			const unsigned char* tpt2, int size2, int* score) override;
	private:		
		void GetParamsDetectFace();
		void GetParamsEnroll();
		void GetParamsControlEntry();
		void CheckError(std::string function, int errorCode);
	private:		
		ASSInterface::Ref<ASSInterface::Configuration> configurationEnroll;
		ASSInterface::Ref<ASSInterface::Configuration> configurationFace;
		ASSInterface::Ref<ASSInterface::Configuration> configurationEntry;
		ASSInterface::Ref<ASSInterface::ErrorIdentification> errorIDkit;
		int indexChannel, detected, extraction, detectForced, multipleFace, detectionThreshold, 
			deduplicationSimilarityThreshold, gallery, deduplicate, concatenate, 
			maxTemplates, concatenateMode,
			minScore, maxScore, identificationSimilarityThreshold, verificationSimilarityThreshold, 
			bestCandidates, identificationSpeed;
		std::string connectDatabase = "iengine.db";
		IENGINE_CONNECTION handleConnect = nullptr;
		IENGINE_USER userEnroll = nullptr;
	};
}
