#pragma once
#include "ASSInterface.h"
#include "data/PersonSpecification.h"

class FrameUnidentified {
public:
	FrameUnidentified();
	~FrameUnidentified();
	void Show(bool* p_open, bool* menuEnroll, int& indexUnidentified);
	void SetPersonUnidentified(PersonSpecification pSpec); 
	inline PersonSpecification* GetPersonUnidentified(int index) { return &listUnidentified[index]; }
	inline void RemovePersonUnidentified(int index) { 
		listUnidentified.erase(listUnidentified.begin() + index); }
	inline void ClearList() { listUnidentified.clear(); }
	inline bool IsEmptyList() { return listUnidentified.empty(); }
private:
	int GetIndexUnidentified(std::string nameControl);
	void SetNewsUnidentified();
	std::vector<std::string> Split(const std::string& s, char delimiter);
private:
	std::vector<PersonSpecification> listUnidentified;	
	std::vector<PersonSpecification> listUnidentifiedTemp;
	std::mutex mutexList;	
};