#pragma once
#include "hzpch.h"

namespace ASSInterface {
	class TransformImage {
	public:
		virtual std::vector<unsigned char> ImenCode(std::vector<unsigned char> flowBytes, int rows, int cols) = 0;
		virtual unsigned char* ImDecode(std::vector<unsigned char> flowBytes) = 0;
		virtual std::vector<unsigned char> Resize(std::vector<unsigned char> flowBytes, int rows, int cols) = 0;
		virtual std::vector<unsigned char> Resize(const char* fileName, int height, int width) = 0;
		virtual bool GetSize(const char* fileName, int* x, int* y) = 0;
		static Ref<TransformImage> Create();
	};
}