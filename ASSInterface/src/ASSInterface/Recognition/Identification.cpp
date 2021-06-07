#include "hzpch.h"
#include "Identification.h"
#include "Platform/Innovatrics/InnoIdentification.h"

namespace ASSInterface {
	Ref<Identification> Identification::CreateInnoIdentify(int channel)
	{
		return CreateRef<InnoIdentification>(channel);
	}
}