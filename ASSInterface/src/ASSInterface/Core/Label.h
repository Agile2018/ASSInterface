#pragma once

#include "hzpch.h"
#include "Core.h"

namespace ASSInterface {

	enum class LanguageType
	{
		enUs = 0,
		esES = 1
	};

	class ASS_API Label {
	public:
		inline static std::string GetLabel(int index, LanguageType lg) { return s_Instance->GetLabelImpl(index, lg); }
		inline static LanguageType GetCurrentLanguage() { return s_Instance->GetCurrentLanguageImpl(); }
	protected:
		virtual std::string GetLabelImpl(int index, LanguageType lg) = 0;
		virtual void Init() = 0;
		virtual std::string CodeUTF8(const std::wstring& wstr) = 0;
		virtual LanguageType GetCurrentLanguageImpl() = 0;
	private:
		static Label* s_Instance;
	};
}