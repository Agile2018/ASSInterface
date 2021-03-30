#pragma once
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameOutputControl {
public:
	FrameOutputControl();
	~FrameOutputControl();
	void Show(bool* p_open);
	inline void SetPersonInside(PersonSpecification pSpec) { listInside.push_back(pSpec); }
	
private:
	inline void RemovePersonInside(int index) { listInside.erase(listInside.begin() + index); }
	void ShowPersonsInside(int index);
	int GetIndexUnidentified(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
private:	
	int indexRemove = -1;
	bool typePersonReadOnly = true;
	std::vector<PersonSpecification> listInside;
};
