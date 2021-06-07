#pragma once
#include "ASSInterface/Core/Core.h"

namespace ASSInterface {
	class ASS_API InitLibrary {
	public:
		virtual void Init() = 0;		
		virtual void Terminate() = 0;
		static Ref<InitLibrary> CreateInnovatrics();
	};
}