#pragma once

#include <string>
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {

	struct EntitySpecification {
		std::string id, name, lastName, dataImage, 
			identification, type, type1, type2, logEnroll, 
			logEntry, dataDocObverse, dataDocReverse;
		long date;
	};

	struct EntityImage {
		std::string id, data;
		int index;
	};

	struct EntityEvent {
		std::string id, description;
		int type;
		long date;
	};

	struct EntityLicence {
		std::string id, code;
		long dateInit, dateEnd;
		int channels, records, galleries;
	};

	enum EventAppType
	{
		enrollmentOnTheFly = 0,
		enrollment = 1,
		inside = 2,
		outside = 3,
		entryWithoutPermission = 4,
		detectedInside = 5,
		unidentified = 6
	};

	class Database {
	public:
		virtual ~Database() = default;
		virtual void Add(const EntitySpecification& ent) = 0;
		virtual void Add(const EntityImage& ent) = 0;
		virtual void Add(const EntityEvent& ent) = 0;
		virtual void Add(const EntityLicence& ent) = 0;
		virtual void Update(const EntitySpecification& ent) = 0;
		virtual void Update(const EntityImage& ent) = 0;
		virtual void Update(const EntityImage& ent, int newIndex) = 0;
		virtual void Update(const EntityEvent& ent) = 0;
		virtual void Update(const EntityLicence& ent) = 0;
		virtual void Delete(const std::string id) = 0;
		virtual void DeleteImages(const std::string id) = 0;
		virtual void DeleteEvent(const std::string id) = 0;
		virtual void DeleteLicence(const std::string id) = 0;		
		virtual void Get(const std::string id) = 0;
		virtual void GetByDocument(const std::string document) = 0;
		virtual void GetImages(const std::string id) = 0;
		virtual void GetEvents(const std::string id) = 0;		
		virtual void GetLicences(const std::string id) = 0;
		virtual void GetByType(const std::string type) = 0;
		virtual void GetByName(const std::string name) = 0;
		virtual void GetByLastName(const std::string lastName) = 0;
		virtual const EntitySpecification& GetEntitySpecification() const = 0;
		virtual const EntityImage& GetEntityImage() const = 0;
		virtual const EntityEvent& GetEntityEvent() const = 0;
		virtual const EntityLicence& GetEntityLicence() const = 0;
		virtual inline const std::vector<EntitySpecification> ListFind() const = 0;
		virtual inline const std::vector<EntityImage> ListImages() const = 0;
		virtual void Drop() = 0;
		virtual void DeleteImage(const EntityImage& ent) = 0;
		static Ref<Database> Create();
	};
}
