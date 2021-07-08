#pragma once

#include <ASSInterface.h>
#include "tbb/tbb.h"

using namespace tbb;

#pragma warning( disable: 588)

namespace ASSInterface {
	class WindowsOpenCV : public TransformImage {
	public:
		WindowsOpenCV();
		virtual ~WindowsOpenCV();
		virtual std::vector<unsigned char> ImenCode(std::vector<unsigned char> flowBytes, int rows, int cols) override;
		virtual std::vector<unsigned char> ImDecode(std::vector<unsigned char> flowBytes) override;
		virtual std::vector<unsigned char> Resize(std::vector<unsigned char> flowBytes, int rows, int cols) override;
		virtual std::vector<unsigned char> Resize(const char* fileName, int height, int width) override;
		virtual unsigned char* Resize(unsigned char* dataImage, int* width,
			int* height, int* lenght) override;
		virtual bool GetSize(const char* fileName, int* x, int* y) override;		
		virtual std::vector<unsigned char> WriteImage(std::vector<unsigned char> dataImage,
			int rows, int cols) override;
		virtual void VerifyImage(unsigned char* data, int size, int height, int width) override;		
		virtual void CaptureDocument(unsigned char* dataImage, int* cols, 
			int* rows, std::vector<unsigned char>& bufferImg) override;
	private:
		
	};
}