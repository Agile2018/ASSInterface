#include "hzpch.h"
#include "JSONConfigOnTheFly.h"

namespace ASSInterface {
	JSONConfigOnTheFly::JSONConfigOnTheFly()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsOnTheFly[AFACE_NUMBER_TEMPLATES] = numberTemplates;
		paramsOnTheFly[AFACE_NUMBER_IMAGES] = showNumberImages;
	}

	JSONConfigOnTheFly::~JSONConfigOnTheFly()
	{
	}
	void JSONConfigOnTheFly::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == AFACE_NUMBER_TEMPLATES) {
					numberTemplates = it.value();
					paramsOnTheFly[AFACE_NUMBER_TEMPLATES] = numberTemplates;
				}
				if (it.key() == AFACE_NUMBER_IMAGES) {
					showNumberImages = it.value();
					paramsOnTheFly[AFACE_NUMBER_IMAGES] = showNumberImages;
				}

			}
		}
	}
	void JSONConfigOnTheFly::ParseToFile()
	{
		numberTemplates = std::any_cast<int>(paramsOnTheFly[AFACE_NUMBER_TEMPLATES]);
		showNumberImages = std::any_cast<int>(paramsOnTheFly[AFACE_NUMBER_IMAGES]);

		nlohmann::json jParams = nlohmann::json::object({ {AFACE_NUMBER_TEMPLATES, numberTemplates},
			{AFACE_NUMBER_IMAGES, showNumberImages }});

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}

	std::any JSONConfigOnTheFly::GetParam(const char* name)
	{
		std::any result;
		if (paramsOnTheFly.find(name) != paramsOnTheFly.end())
			return paramsOnTheFly[name];
		return result;
	}

	void JSONConfigOnTheFly::SetParam(const char* name, std::any value)
	{
		paramsOnTheFly[name] = value;
	}
	
	void JSONConfigOnTheFly::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigOnTheFly::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}