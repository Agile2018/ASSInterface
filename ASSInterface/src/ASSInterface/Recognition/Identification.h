#pragma once
#include "hzpch.h"
#include "Detection.h"

namespace ASSInterface {
	struct IdentitySpecification {
		int id;
		std::vector<std::string> data;
		int task; // 0->new 1->get
	};

	class Identification {
	public:
		virtual void LoadConnection() = 0;
		virtual void CloseConnection() = 0;
		virtual void Enroll(const DetectSpecification& spec) = 0;
		virtual void Control(const DetectSpecification& spec) = 0;
		virtual const IdentitySpecification& Get() const = 0;
	private:
		virtual int Find(const unsigned char* tpt, int size, int* id, int* score) = 0;
		virtual int Match(const unsigned char* tpt1, int size1, 
			const unsigned char* tpt2, int size2, int* score) = 0;

	};
}
