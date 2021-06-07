#include "hzpch.h"
#include "InitLibrary.h"
#include "Platform/Innovatrics/InnoInitLibrary.h"

namespace ASSInterface {
	Ref<InitLibrary> InitLibrary::CreateInnovatrics()
	{
		return CreateRef<InnoInitLibrary>();
	}
}