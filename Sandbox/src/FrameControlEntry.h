#pragma once

#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameControlEntry {
public:
	FrameControlEntry();
	~FrameControlEntry();
	void Show(bool* p_open, PersonSpecification& personSpec);
	inline void SetViewChannel(bool* isView) { viewVideo.push_back(isView); }
	inline void SetNameOperator(std::string name) { nameOperator = name; }
	inline PersonSpecification* GetPersonLasAction() { return &personSpecificationLastAction; }
	inline void SetPersonUnidentified(PersonSpecification pSpec) { listUnidentified.push_back(pSpec); }
	inline void SetPersonDetected(PersonSpecification pSpec) { listDetected.push_back(pSpec); }
	inline PersonSpecification* GetPersonUnidentified(int index) { return &listUnidentified[index]; }
private:
	void ShowScreenHead();
	void ShowLastAction();
	void ShowUnidentified(bool* menuEnroll, int& indexUnidentified);
	void ShowDetected(int index, PersonSpecification& personSpec);
	int GetIndexDetected(std::string nameControl);
	std::vector<std::string> Split(const std::string& s, char delimiter);
	inline void RemovePersonDetected(int index) { listDetected.erase(listDetected.begin() + index); }
	void SetPersonInside(int index, PersonSpecification& personSpec);
private:
	const char* itemsTarget[2] = { "Office 1", "Office 2" };
	bool typePersonReadOnly = false;
	std::vector<bool*> viewVideo;
	std::string nameOperator = "Operator";
	int indexRemove = -1;
	PersonSpecification personSpecificationLastAction;
	std::vector<PersonSpecification> listUnidentified;
	std::vector<PersonSpecification> listDetected;
};