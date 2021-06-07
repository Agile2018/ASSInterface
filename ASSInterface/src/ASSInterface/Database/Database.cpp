#include "hzpch.h"
#include "Database.h"

#include "Platform/MongoDB/MongoDBASS.h"

namespace ASSInterface {
	Ref<Database> Database::Create()
	{
		return CreateRef<MongoDBASS>();
	}
}