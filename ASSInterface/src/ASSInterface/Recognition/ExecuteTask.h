#pragma once

#include "hzpch.h"
#include "ASSInterface.h"

namespace ASSInterface {
	class ExecuteTask {
	public:
		virtual ~ExecuteTask() = default;
		virtual inline const Rx::observable<IdentitySpecification> Get() const = 0;
		virtual inline const Rx::observable<IdentitySpecification> GetEdition() const = 0;
		virtual inline const Rx::observable<int> EndTask() const = 0;
		virtual inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) = 0;
		virtual void CloseConnection() = 0;
		virtual void ResetParameters() = 0;
		virtual void DoTask(concurrent_vector<DetectSpecification> concurrentDetection, std::string gallery) = 0;
		virtual void DoTask(DetectSpecification specDetect) = 0;
		virtual void DoTask(std::vector<DetectSpecification> specDetects) = 0;
		virtual void Enroll(TemplateSpecification specTemplate, int* id) = 0;
		virtual void EnrollTemplates(std::vector<TemplateSpecification> specTemplate, int* id) = 0;
		virtual void Remove(int id) = 0;
		virtual void Import(concurrent_vector<DetectSpecification> concurrentDetection, std::string gallery) = 0;
		virtual int GetFaceCount(int id) = 0;
		virtual std::vector<int> GetFaceQuality(int id, int index) = 0;
		virtual void RemoveFace(int id, int index) = 0;
		virtual inline void SetId(int id) = 0;
		virtual inline void SetIndex(int index) = 0;
		static Ref<ExecuteTask> CreateInnoTask(int channel);
	};
}