#include "hzpch.h"
#include "MongoDBASS.h"

namespace ASSInterface {
	MongoDBASS::MongoDBASS(const std::string& connection)
	{				
		mongocxx::uri uri(connection.c_str()); //"mongodb://localhost:27017"
		class NoopLogger : public mongocxx::logger {
		public:
			virtual void operator()(mongocxx::log_level,
				bsoncxx::stdx::string_view,
				bsoncxx::stdx::string_view) noexcept {}
		};

		auto instance =
			bsoncxx::stdx::make_unique<mongocxx::instance>(bsoncxx::stdx::make_unique<NoopLogger>());

		MongoAccess::instance().Configure(std::move(instance),
			bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));
	}

	MongoDBASS::~MongoDBASS()
	{
	}

	void MongoDBASS::Add(const EntitySpecification& ent)
	{
		try
		{
			auto clientConnection = MongoAccess::instance().GetConnection();

			mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

			mongocxx::collection collection = database[COLLECTION_PERSON.c_str()];
			
			bsoncxx::document::value builder = make_document(
				kvp("id_face", ent.id.c_str()),
				kvp("name", ent.name.c_str()),
				kvp("date", ent.date),
				kvp("identification", ent.identification.c_str()),
				kvp("type", ent.type.c_str()),
				kvp("log_enroll", ent.logEnroll.c_str()),
				kvp("log_entry", ent.logEntry.c_str()),
				kvp("data", ent.data.c_str()));

			bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));
			
		}
		catch (const mongocxx::exception& e)
		{
			ASS_ERROR("Error Mongo Add: {0}", e.what());

		}
	}

	void MongoDBASS::Add(const EntityImage& ent)
	{
		try
		{
			auto clientConnection = MongoAccess::instance().GetConnection();

			mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

			mongocxx::collection collection = database[COLLECTION_IMAGES.c_str()];

			bsoncxx::document::value builder = make_document(
				kvp("id_face", ent.id.c_str()),
				kvp("data1", ent.data1.c_str()),
				kvp("data2", ent.data2.c_str()),
				kvp("data3", ent.data3.c_str()),
				kvp("data4", ent.data4.c_str()),
				kvp("data5", ent.data5.c_str()),
				kvp("data6", ent.data6.c_str()),
				kvp("data7", ent.data7.c_str()),
				kvp("data8", ent.data8.c_str()),
				kvp("data9", ent.data9.c_str()),
				kvp("data10", ent.data10.c_str()));

			bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));

		}
		catch (const mongocxx::exception& e)
		{
			ASS_ERROR("Error Mongo Add: {0}", e.what());

		}
	}

	void MongoDBASS::Add(const EntityEvent& ent)
	{
		try
		{
			auto clientConnection = MongoAccess::instance().GetConnection();

			mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

			mongocxx::collection collection = database[COLLECTION_EVENTS.c_str()];

			bsoncxx::document::value builder = make_document(
				kvp("id_face", ent.id.c_str()),
				kvp("type", ent.type.c_str()),
				kvp("date", ent.date));

			bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));

		}
		catch (const mongocxx::exception& e)
		{
			ASS_ERROR("Error Mongo Add: {0}", e.what());

		}
	}

	void MongoDBASS::Update(const EntitySpecification& ent)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end())
		{
			mongocxx::collection collection = database[COLLECTION_PERSON.c_str()];

			try
			{

				bsoncxx::stdx::optional<mongocxx::v_noabi::result::update> result = collection
					.update_one(make_document(kvp("id_face", ent.id.c_str())),
						make_document(kvp("$set",
							make_document(kvp("name", ent.name.c_str()),
								kvp("date", ent.date),
								kvp("identification", ent.identification.c_str()),
								kvp("type", ent.type.c_str()),
								kvp("log_enroll", ent.logEnroll.c_str()),
								kvp("log_entry", ent.logEntry.c_str()),
								kvp("data", ent.data.c_str())))));
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
		
	}

	void MongoDBASS::Update(const EntityImage& ent)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end())
		{
			mongocxx::collection collection = database[COLLECTION_IMAGES.c_str()];

			try
			{

				bsoncxx::stdx::optional<mongocxx::v_noabi::result::update> result = collection
					.update_one(make_document(kvp("id_face", ent.id.c_str())),
						make_document(kvp("$set",
							make_document(
								kvp("data1", ent.data1.c_str()),
								kvp("data2", ent.data2.c_str()),
								kvp("data3", ent.data3.c_str()),
								kvp("data4", ent.data4.c_str()),
								kvp("data5", ent.data5.c_str()),
								kvp("data6", ent.data6.c_str()),
								kvp("data7", ent.data7.c_str()),
								kvp("data8", ent.data8.c_str()),
								kvp("data9", ent.data9.c_str()),
								kvp("data10", ent.data10.c_str())))));
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::Update(const EntityEvent& ent)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end())
		{
			mongocxx::collection collection = database[COLLECTION_EVENTS.c_str()];

			try
			{

				bsoncxx::stdx::optional<mongocxx::v_noabi::result::update> result = collection
					.update_one(make_document(kvp("id_face", ent.id.c_str())),
						make_document(kvp("$set",
							make_document(
								kvp("type", ent.type.c_str()),
								kvp("date", ent.date)))));
								
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::Delete(const std::string id)
	{
		
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_PERSON.c_str()];

				bsoncxx::document::value builder = make_document(
					kvp("id_face", id.c_str()));

				boost::optional<mongocxx::v_noabi::result::delete_result> result = collection.delete_one(std::move(builder));

				if (result) {
					ASS_INFO("Records delete: {0}", result->deleted_count());
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());

			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::DeleteImages(const std::string id)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_IMAGES.c_str()];

				bsoncxx::document::value builder = make_document(
					kvp("id_face", id.c_str()));

				boost::optional<mongocxx::v_noabi::result::delete_result> result = collection.delete_one(std::move(builder));

				if (result) {
					ASS_INFO("Records delete: {0}", result->deleted_count());
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());

			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::DeleteEvent(const std::string id)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursor = database.list_collections();

		if (cursor.begin() != cursor.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_EVENTS.c_str()];

				bsoncxx::document::value builder = make_document(
					kvp("id_face", id.c_str()));

				boost::optional<mongocxx::v_noabi::result::delete_result> result = collection.delete_one(std::move(builder));

				if (result) {
					ASS_INFO("Records delete: {0}", result->deleted_count());
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Update: {0}", e.what());

			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::Get(const std::string id)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursorCheck = database.list_collections();

		if (cursorCheck.begin() != cursorCheck.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_PERSON.c_str()];
				
				boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
					.find_one(make_document(kvp("id_face", id.c_str())));

				if (cursor)
				{									
					auto view = (*cursor).view();

					if (!view.empty()) {
						
						m_EntitySpec.id = view["id_face"].get_utf8().value.to_string();
						m_EntitySpec.name = view["name"].get_utf8().value.to_string();
						m_EntitySpec.identification = view["identification"].get_utf8().value.to_string();
						m_EntitySpec.data = view["data"].get_utf8().value.to_string();
						m_EntitySpec.type = view["type"].get_utf8().value.to_string();
						m_EntitySpec.logEnroll = view["log_enroll"].get_utf8().value.to_string();
						m_EntitySpec.logEntry = view["log_entry"].get_utf8().value.to_string();
						m_EntitySpec.date = view["date"].get_int32().value;

					}
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Get Entity: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
		
	}
	void MongoDBASS::GetImages(const std::string id)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursorCheck = database.list_collections();

		if (cursorCheck.begin() != cursorCheck.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_IMAGES.c_str()];

				boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
					.find_one(make_document(kvp("id_face", id.c_str())));

				if (cursor)
				{
					auto view = (*cursor).view();

					if (!view.empty()) {

						m_EntityImage.id = view["id_face"].get_utf8().value.to_string();
						m_EntityImage.data1 = view["data1"].get_utf8().value.to_string();
						m_EntityImage.data2 = view["data2"].get_utf8().value.to_string();
						m_EntityImage.data3 = view["data3"].get_utf8().value.to_string();
						m_EntityImage.data4 = view["data4"].get_utf8().value.to_string();
						m_EntityImage.data5 = view["data5"].get_utf8().value.to_string();
						m_EntityImage.data6 = view["data6"].get_utf8().value.to_string();
						m_EntityImage.data7 = view["data7"].get_utf8().value.to_string();
						m_EntityImage.data8 = view["data8"].get_utf8().value.to_string();
						m_EntityImage.data9 = view["data9"].get_utf8().value.to_string();
						m_EntityImage.data10 = view["data10"].get_utf8().value.to_string();

					}
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Get Entity Images: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}

	void MongoDBASS::GetEvents(const std::string id)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();

		mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];

		auto cursorCheck = database.list_collections();

		if (cursorCheck.begin() != cursorCheck.end()) {

			try
			{
				mongocxx::collection collection = database[COLLECTION_EVENTS.c_str()];

				boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
					.find_one(make_document(kvp("id_face", id.c_str())));

				if (cursor)
				{
					auto view = (*cursor).view();

					if (!view.empty()) {

						m_EntityEvent.id = view["id_face"].get_utf8().value.to_string();						
						m_EntityEvent.type = view["type"].get_utf8().value.to_string();						
						m_EntityEvent.date = view["date"].get_int32().value;

					}
				}
			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Get Entity Events: {0}", e.what());
			}
		}
		else
		{
			ASS_ERROR("Error collections not created");
		}
	}


}