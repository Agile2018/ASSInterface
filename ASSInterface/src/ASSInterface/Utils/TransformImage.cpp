#include "hzpch.h"

#include "TransformImage.h"
#include "Platform/Windows/WindowsOpenCV.h"

namespace ASSInterface {

	Ref<TransformImage> TransformImage::Create()
	{
		return CreateRef<WindowsOpenCV>();
	}

}