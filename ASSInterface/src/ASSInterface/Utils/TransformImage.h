#pragma once
#include "hzpch.h"
#include <ASSInterface.h>

namespace ASSInterface {
	class TransformImage {
	public:
		virtual std::vector<unsigned char> ImenCode(std::vector<unsigned char> flowBytes, int rows, int cols) = 0;
		virtual std::vector<unsigned char> ImDecode(std::vector<unsigned char> flowBytes) = 0;
		virtual std::vector<unsigned char> Resize(std::vector<unsigned char> flowBytes, int rows, int cols) = 0;
		virtual std::vector<unsigned char> Resize(const char* fileName, int height, int width) = 0;		
		virtual void CaptureDocument(unsigned char* dataImage, int* cols, 
			int* rows, std::vector<unsigned char>& bufferImg) = 0;
		virtual unsigned char* Resize(unsigned char* dataImage, int* width, 
			int* height, int* lenght) = 0;
		virtual bool GetSize(const char* fileName, int* x, int* y) = 0;		
		virtual std::vector<unsigned char> WriteImage(std::vector<unsigned char> dataImage,
			int rows, int cols) = 0;
		virtual void VerifyImage(unsigned char* data, int size, int height, int width) = 0;
		static Ref<TransformImage> Create();
	};
}