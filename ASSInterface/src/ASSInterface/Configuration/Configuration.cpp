#include "hzpch.h"
#include "Configuration.h"
#include "Platform/JSON/JSONConfigDatabase.h"
#include "Platform/JSON/JSONConfigVideo.h"

namespace ASSInterface {
	Ref<Configuration> Configuration::CreateConfigDatabase()
	{
		return CreateRef<JSONConfigDatabase>();
	}
	Ref<Configuration> Configuration::CreateConfigVideo(std::string nameFile)
	{
		return CreateRef<JSONConfigVideo>(nameFile);
	}
}