#pragma once
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameUnidentified {
public:
	FrameUnidentified();
	~FrameUnidentified();
	void Show(bool* p_open, bool* menuEnroll, int& indexUnidentified);
	inline void SetPersonUnidentified(PersonSpecification pSpec) { listUnidentified.push_back(pSpec); }
	inline PersonSpecification* GetPersonUnidentified(int index) { return &listUnidentified[index]; }
	inline void RemovePersonUnidentified(int index) { listUnidentified.erase(listUnidentified.begin() + index); }
private:
	int GetIndexUnidentified(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
private:
	std::vector<PersonSpecification> listUnidentified;
};