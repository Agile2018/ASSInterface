#include "hzpch.h"
#include "Base64.h"
#include "Platform/Windows/WindowsBase64.h"

namespace ASSInterface {
	Ref<Base64> Base64::Create()
	{
		return CreateRef<WindowsBase64>();
	}
}