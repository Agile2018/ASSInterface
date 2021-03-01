#pragma once

#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigDatabase : public Configuration
	{
	public:
		JSONConfigDatabase();
		virtual ~JSONConfigDatabase();			
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;		
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
	private:		
		virtual void SaveFile(std::string content) override;
	private:				
		std::string path;
		const std::string fileDatabase = "database.txt";
		const std::string NAME_DATABASE = "name";
		const std::string CONNECT_STRING = "connect";
		std::string nameDatabase = "dbFace";
		std::string connectString = "mongodb://192.168.0.9:27017/?minPoolSize=3&maxPoolSize=3";
		mutable std::unordered_map<std::string, std::any> paramsDatabase;
		ASSInterface::Ref<ASSInterface::File> managerFile;
		
	};
}
