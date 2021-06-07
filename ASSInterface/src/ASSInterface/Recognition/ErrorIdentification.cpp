#include "hzpch.h"
#include "ErrorIdentification.h"
#include "Platform/Innovatrics/InnoErrorIDkit.h"
namespace ASSInterface {
	Ref<ErrorIdentification> ErrorIdentification::CreateIDkit()
	{
		return CreateRef<InnoErrorIDkit>();
	}
}