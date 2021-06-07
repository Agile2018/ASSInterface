#pragma once

#include "hzpch.h"
#include "ASSInterface.h"

namespace ASSInterface {
	class ExecuteTask {
	public:
		virtual ~ExecuteTask() = default;
		virtual inline const Rx::observable<IdentitySpecification> Get() const = 0;
		virtual inline const Rx::observable<int> EndTask() const = 0;
		virtual inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) = 0;
		virtual void CloseConnection() = 0;
		virtual void ResetParameters() = 0;
		virtual void DoTask(concurrent_vector<DetectSpecification> concurrentDetection) = 0;
		virtual void DoTask(DetectSpecification specDetect) = 0;
		virtual void DoTask(std::vector<DetectSpecification> specDetects) = 0;
		virtual void Enroll(std::vector<char> templateFace, int* id) = 0;
		virtual void EnrollTemplates(std::vector<TemplateSpecification> specTemplate, int* id) = 0;
		virtual void Remove(int id) = 0;
		virtual void Import(concurrent_vector<DetectSpecification> concurrentDetection) = 0;
		static Ref<ExecuteTask> CreateInnoTask(int channel);
	};
}