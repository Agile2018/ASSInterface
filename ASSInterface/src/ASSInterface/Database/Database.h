#pragma once

#include <string>
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {

	struct EntitySpecification {
		std::string id, name, data, identification, type, logEnroll, logEntry;
		long date;

	};

	struct EntityImage {
		std::string id, data1, data2, data3, data4, 
			data5, data6, data7, data8, data9, data10;
	};

	struct EntityEvent {
		std::string id, type;
		long date;
	};

	class Database {
	public:
		virtual ~Database() = default;
		virtual void Add(const EntitySpecification& ent) = 0;
		virtual void Add(const EntityImage& ent) = 0;
		virtual void Add(const EntityEvent& ent) = 0;
		virtual void Update(const EntitySpecification& ent) = 0;
		virtual void Update(const EntityImage& ent) = 0;
		virtual void Update(const EntityEvent& ent) = 0;
		virtual void Delete(const std::string id) = 0;
		virtual void DeleteImages(const std::string id) = 0;
		virtual void DeleteEvent(const std::string id) = 0;
		virtual void SetDatabase(const std::string& database) = 0;
		virtual void Get(const std::string id) = 0;
		virtual void GetImages(const std::string id) = 0;
		virtual void GetEvents(const std::string id) = 0;		
		virtual const EntitySpecification& GetEntitySpecification() const = 0;
		virtual const EntityImage& GetEntityImage() const = 0;
		virtual const EntityEvent& GetEntityEvent() const = 0;
		virtual void Drop() = 0;
		static Ref<Database> Create(const std::string& connection);
	};
}
