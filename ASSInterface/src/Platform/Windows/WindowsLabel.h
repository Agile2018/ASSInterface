#pragma once

#include "ASSInterface/Core/Label.h"

namespace ASSInterface {
	class WindowsLabel : public Label {		
	protected:
		virtual std::string GetLabelImpl(int index, LanguageType lg) override;
		virtual void Init() override;
		virtual std::string CodeUTF8(const std::wstring& wstr) override;
		virtual LanguageType GetCurrentLanguageImpl() override;
	private:
		std::vector<std::vector<std::wstring>> languages;
	};
}