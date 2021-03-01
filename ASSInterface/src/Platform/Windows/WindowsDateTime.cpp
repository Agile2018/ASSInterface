#include "hzpch.h"
#include "ASSInterface/Utils/DateTime.h"

namespace ASSInterface {
	std::string DateTime::Now() {
		char buff[20];
		time_t now = time(NULL);
		strftime(buff, 20, "%Y-%m-%d %H:%M", localtime(&now));
		std::string timeNow(buff);		
		return timeNow;
	}

	long DateTime::NowToLong() {
		struct tm* timeinfo;
		time_t now = time(NULL);
		timeinfo = localtime(&now);
		return static_cast<long>(now);
	}

	std::string DateTime::LongToString(long longDate) {
		char buff[20];
		time_t longToTime = longDate;
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&longToTime));
		std::string newDate(buff);
		return newDate;
	}
}