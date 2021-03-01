#pragma once

#include "hzpch.h"

#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>

namespace ASSInterface {
	class MongoAccess {
	public:
		static MongoAccess& instance() {
			static MongoAccess instance;
			return instance;
		}

		void Configure(std::unique_ptr<mongocxx::instance> inputInstance,
			std::unique_ptr<mongocxx::pool> inputPool) {
			try
			{
				instanceMain = std::move(inputInstance);
				poolMain = std::move(inputPool);

			}
			catch (const mongocxx::exception& e)
			{
				ASS_ERROR("Error Mongo Configure: {0}", e.what());
				
			}
		}
		using connection = mongocxx::pool::entry;

		connection GetConnection() {
			return poolMain->acquire();
		}

		bsoncxx::stdx::optional<connection> TryGetConnection() {
			return poolMain->try_acquire();
		}

	private:
		MongoAccess() = default;
		std::unique_ptr<mongocxx::instance> instanceMain = nullptr;
		std::unique_ptr<mongocxx::pool> poolMain = nullptr;
	};
}
