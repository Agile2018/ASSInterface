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
		MongoDBASS();
		virtual ~MongoDBASS();
		virtual void Add(const EntitySpecification& ent) override;
		virtual void Add(const EntityImage& ent) override;
		virtual void Add(const EntityEvent& ent) override;
		virtual void Add(const EntityLicence& ent) override;
		virtual void Update(const EntitySpecification& ent) override;
		virtual void Update(const EntityImage& ent) override;
		virtual void Update(const EntityEvent& ent) override;
		virtual void Update(const EntityLicence& ent) override;
		virtual void Delete(const std::string id) override;
		virtual void DeleteImages(const std::string id) override;
		virtual void DeleteEvent(const std::string id) override;
		virtual void DeleteLicence(const std::string id) override;		
		virtual const EntitySpecification& GetEntitySpecification() const override { return m_EntitySpec; };
		virtual const EntityImage& GetEntityImage() const override { return m_EntityImage; };
		virtual const EntityEvent& GetEntityEvent() const override { return m_EntityEvent; };
		virtual const EntityLicence& GetEntityLicence() const override { return m_EntityLicence; };
		virtual void Get(const std::string id) override;
		virtual void GetByDocument(const std::string document) override;
		virtual void GetImages(const std::string id) override;
		virtual void GetEvents(const std::string id) override;
		virtual void GetLicences(const std::string id) override;
		virtual void Drop() override;
	private:
		void GetParametersDb();
	private:
		const std::string COLLECTION_PERSON = "person";
		const std::string COLLECTION_IMAGES = "images";
		const std::string COLLECTION_EVENTS = "events";
		const std::string COLLECTION_LICENCES = "licences";
		ASSInterface::Ref<ASSInterface::Configuration> configurationDB;
		std::string nameDatabase;
		std::string connection;
		EntitySpecification m_EntitySpec;
		EntityImage m_EntityImage;
		EntityEvent m_EntityEvent;
		EntityLicence m_EntityLicence;
	};
}
