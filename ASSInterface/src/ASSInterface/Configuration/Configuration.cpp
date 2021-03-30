#include "hzpch.h"
#include "Configuration.h"
#include "Platform/JSON/JSONConfigDatabase.h"
#include "Platform/JSON/JSONConfigVideo.h"
#include "Platform/JSON/JSONConfigGlobal.h"
#include "Platform/JSON/JSONConfigFace.h"
#include "Platform/JSON/JSONConfigTrack.h"
#include "Platform/JSON/JSONConfigEnroll.h"
#include "Platform/JSON/JSONConfigEntry.h"
#include "Platform/JSON/JSONConfigOnTheFly.h"

namespace ASSInterface {
	Ref<Configuration> Configuration::CreateConfigDatabase()
	{
		return CreateRef<JSONConfigDatabase>();
	}
	Ref<Configuration> Configuration::CreateConfigVideo(std::string nameFile)
	{
		return CreateRef<JSONConfigVideo>(nameFile);
	}
	Ref<Configuration> Configuration::CreateConfigGlobal()
	{
		return CreateRef<JSONConfigGlobal>();
	}
	Ref<Configuration> Configuration::CreateConfigFace()
	{
		return CreateRef<JSONConfigFace>();
	}
	Ref<Configuration> Configuration::CreateConfigTrack()
	{
		return CreateRef<JSONConfigTrack>();
	}
	Ref<Configuration> Configuration::CreateConfigEnroll()
	{
		return CreateRef<JSONConfigEnroll>();
	}
	Ref<Configuration> Configuration::CreateConfigEntry()
	{
		return CreateRef<JSONConfigEntry>();
	}
	Ref<Configuration> Configuration::CreateConfigOnTheFly()
	{
		return CreateRef<JSONConfigOnTheFly>();
	}
}