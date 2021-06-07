#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
#include "data/PersonSpecification.h"
#include "data/ConstantApplication.h"

class FrameEditRegister {
public:
	FrameEditRegister(ASSInterface::LanguageType language);
	~FrameEditRegister();
	void Show(bool* p_open);
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	void Search();
	void Save();
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
private:
	ASSInterface::Ref<ASSInterface::Database> dbMongo;
	PersonSpecification personSpecificationEdit;
	ASSInterface::LanguageType lg;
	ASSInterface::Ref<ASSInterface::Base64> base64;
	ASSInterface::EntitySpecification m_EntitySpec;
	const char* currentItemPersonType = NULL;
	std::string* messageStatus;
};
