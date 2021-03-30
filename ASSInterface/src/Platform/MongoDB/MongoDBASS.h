#pragma once

#include <ASSInterface.h>
#include "MongoAccess.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::type;

namespace ASSInterface {

	class MongoDBASS : public Database {
	public:
		MongoDBASS(const std::string& connection);
		virtual ~MongoDBASS();
		virtual void Add(const EntitySpecification& ent) override;
		virtual void Add(const EntityImage& ent) override;
		virtual void Add(const EntityEvent& ent) override;
		virtual void Update(const EntitySpecification& ent) override;
		virtual void Update(const EntityImage& ent) override;
		virtual void Update(const EntityEvent& ent) override;
		virtual void Delete(const std::string id) override;
		virtual void DeleteImages(const std::string id) override;
		virtual void DeleteEvent(const std::string id) override;
		virtual void SetDatabase(const std::string& database) { nameDatabase = database; };
		virtual const EntitySpecification& GetEntitySpecification() const override { return m_EntitySpec; };
		virtual const EntityImage& GetEntityImage() const override { return m_EntityImage; };
		virtual const EntityEvent& GetEntityEvent() const override { return m_EntityEvent; };
		virtual void Get(const std::string id) override;
		virtual void GetImages(const std::string id) override;
		virtual void GetEvents(const std::string id) override;
		virtual void Drop() override;
	private:
		const std::string COLLECTION_PERSON = "person";
		const std::string COLLECTION_IMAGES = "images";
		const std::string COLLECTION_EVENTS = "events";
		std::string nameDatabase;
		EntitySpecification m_EntitySpec;
		EntityImage m_EntityImage;
		EntityEvent m_EntityEvent;
	};
}
