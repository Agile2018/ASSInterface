#pragma once
#include "hzpch.h"
#include "Detection.h"

namespace ASSInterface {
	struct IdentitySpecification {
		int id;		
		int cropLength;
		int cropWidth;
		int cropHeight;
		std::vector<unsigned char> cropData;		
		std::string imageDB;
		int channel;
		std::string name;	
		std::string document;
		std::string lastName;
		std::string docObverse;
		std::string docReverse;
		std::vector<char> templateData;
		std::vector<char> faceSerialized;
		int size;
		int match;
		std::string gallery;
		std::string gallery1;
		int task;
		float medianICAO = -1.0f;

		IdentitySpecification() {
			id = -1;			
			cropLength = 0;
			cropWidth = 0;
			cropHeight = 0;
			imageDB = "";
			channel = -1;
			name = "";
			document = "";
			docObverse = "";
			docReverse = "";
			size = 0;
			match = 0;
			gallery = "";
			gallery1 = "";
			task = 0;
			medianICAO = -1.0f;
			cropData.clear();
			templateData.clear();
			faceSerialized.clear();
		}
	};

	struct TemplateSpecification {
		std::vector<char> templateData;
		std::vector<char> faceSerialized;
		std::vector<unsigned char> bufferImage;
		std::string imageDB;
		uint32_t txtImage = 0;
		int score;
		int quality;
		int size;
		int index;
		TemplateSpecification() {
			size = 0;
			quality = 0;
			score = 0;
			index = -1;
		}
	};

	class Identification {
	public:
		virtual void ResetParameters() = 0;
		virtual void SetParameters(int option) = 0;
		virtual void LoadConnection(int option) = 0;
		virtual void CloseConnection() = 0;
		virtual void Enroll(const unsigned char* tpt,
			int size, int* id) = 0;
		virtual void EnrollImport(const unsigned char* tpt,
			int size, const unsigned char* data, int length, int* id) = 0;

		virtual void EnrollWithData(const unsigned char* tpt, int size,
			const unsigned char* data, int dataLength, int* id) = 0;
		virtual void ClearEnroll() = 0;
		virtual void AddTemplate(const unsigned char* tpt, int size) = 0;
		virtual void AddTemplate(const unsigned char* tpt,
			int size, int id) = 0;
		virtual unsigned char* GetTemplate(int id, int* lenght, int index) = 0;
		virtual void Remove(int id) = 0;
		virtual void Find(const unsigned char* tpt, int size, int* id, int* score) = 0;
		virtual void Match(const unsigned char* tpt1, int size1,
			const unsigned char* tpt2, int size2, int* score) = 0;
		virtual int GetFaceCount(int id) = 0;
		virtual void SetCustomData(const unsigned char* data, int length) = 0;
		virtual void GetFaceQuality(int id, int index, int* quality) = 0;
		virtual void RemoveFace(int id, int index) = 0;
		virtual void SetFaceTemplate(int id, int index, const unsigned char* tpt, int length) = 0;
		static Ref<Identification> CreateInnoIdentify(int channel);
	
	};
}
