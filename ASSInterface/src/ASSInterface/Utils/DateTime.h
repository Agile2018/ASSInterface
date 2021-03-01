#pragma once

#include "hzpch.h"

namespace ASSInterface {
	class DateTime {
	public:
		static std::string Now();
		static long NowToLong();
		static std::string LongToString(long longDate);
	};
}
