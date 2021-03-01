#include "hzpch.h"
#include "Database.h"

#include "Platform/MongoDB/MongoDBASS.h"

namespace ASSInterface {
	Ref<Database> Database::Create(const std::string& connection)
	{
		return CreateRef<MongoDBASS>(connection);
	}
}