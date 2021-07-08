#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameOutputControl {
public:
	FrameOutputControl();
	~FrameOutputControl();
	void Show(bool* p_open);
	inline void SetPersonInside(PersonSpecification pSpec) { listInside.push_back(pSpec); }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	inline Rx::observable<int> GetPersonOurside() {
		return observablePersonOutside;
	}
private:
	inline void RemovePersonInside(int index) { listInside.erase(listInside.begin() + index); }
	void ShowPersonsInside(int index);
	void ShowScreenHead();
	int GetIndexOut(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
	void SaveEventOutside(int index);
private:	
	Rx::subject<int> personSubject;
	Rx::observable<int> observablePersonOutside = personSubject.get_observable();
	Rx::subscriber<int> shootPersonOutside = personSubject.get_subscriber();
	int indexRemove = -1;
	bool typePersonReadOnly = true;
	std::vector<PersonSpecification> listInside;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
};
