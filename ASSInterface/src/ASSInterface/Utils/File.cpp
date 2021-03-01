#include "hzpch.h"
#include "File.h"
#include "Platform/Windows/WindowsFile.h"

namespace ASSInterface {
	Ref<File> File::Create()
	{
		return CreateRef<WindowsFile>();
	}
}