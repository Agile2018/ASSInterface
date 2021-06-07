#pragma once
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameOutputControl {
public:
	FrameOutputControl();
	~FrameOutputControl();
	void Show(bool* p_open);
	inline void SetPersonInside(PersonSpecification pSpec) { listInside.push_back(pSpec); }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
private:
	inline void RemovePersonInside(int index) { listInside.erase(listInside.begin() + index); }
	void ShowPersonsInside(int index);
	void ShowScreenHead();
	int GetIndexOut(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
	void SaveEventOutside(int index);
private:	
	int indexRemove = -1;
	bool typePersonReadOnly = true;
	std::vector<PersonSpecification> listInside;
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
};
