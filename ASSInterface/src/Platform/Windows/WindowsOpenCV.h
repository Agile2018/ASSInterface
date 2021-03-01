#pragma once

#include "ASSInterface/Utils/TransformImage.h"

namespace ASSInterface {
	class WindowsOpenCV : public TransformImage {
	public:
		WindowsOpenCV();
		virtual ~WindowsOpenCV();
		virtual std::vector<unsigned char> ImenCode(std::vector<unsigned char> flowBytes, int rows, int cols) override;
		virtual unsigned char* ImDecode(std::vector<unsigned char> flowBytes) override;
		virtual std::vector<unsigned char> Resize(std::vector<unsigned char> flowBytes, int rows, int cols) override;
		virtual std::vector<unsigned char> Resize(const char* fileName, int height, int width) override;
		virtual bool GetSize(const char* fileName, int* x, int* y) override;
	};
}