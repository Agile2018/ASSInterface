#include "hzpch.h"
#include "JSONConfigDatabase.h"

namespace ASSInterface {
	JSONConfigDatabase::JSONConfigDatabase()
	{		
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() +  "/" + fileDatabase;
		paramsDatabase[NAME_DATABASE] = nameDatabase;
		paramsDatabase[CONNECT_STRING] = connectString;
	}
	JSONConfigDatabase::~JSONConfigDatabase()
	{
	}
	void JSONConfigDatabase::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}	

	void JSONConfigDatabase::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		auto jdata = nlohmann::json::parse(strJson);
		nameDatabase = jdata["name"];
		connectString = jdata["connect"];
		paramsDatabase[NAME_DATABASE] = nameDatabase;
		paramsDatabase[CONNECT_STRING] = connectString;
	}
	void JSONConfigDatabase::ParseToFile()
	{
		nlohmann::json jParams = nlohmann::json::object({ {NAME_DATABASE, nameDatabase}, {CONNECT_STRING, connectString} });
		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
		
	}
	std::any JSONConfigDatabase::GetParam(const char* name)
	{
		std::any result;
		if (paramsDatabase.find(name) != paramsDatabase.end())
			return paramsDatabase[name];	
		return result;
	}

	void JSONConfigDatabase::SetParam(const char* name, std::any value)
	{
		paramsDatabase[name] = value;
	}
	
}