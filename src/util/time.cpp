//
//  time.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "time.h"

#include <time.h>

#if WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace kge
{
	long long Time::_time_startup = 0;

	long long Time::GetTimeMS()
	{
		long long t;

#if WIN32
		SYSTEMTIME sys_time;
		::GetLocalTime(&sys_time);

		struct tm tm;
		tm.tm_sec = sys_time.wSecond;
		tm.tm_min = sys_time.wMinute;
		tm.tm_hour = sys_time.wHour;
		tm.tm_mday = sys_time.wDay;
		tm.tm_mon = sys_time.wMonth - 1;
		tm.tm_year = sys_time.wYear - 1900;
		tm.tm_isdst = -1;

		t = mktime(&tm) * (long long)1000 + sys_time.wMilliseconds;
#else
		struct timeval tv;
		gettimeofday(&tv, NULL);
		t = tv.tv_sec;
		t *= 1000;
		t += tv.tv_usec / 1000;
	#endif

		return t;
	}

	float Time::GetTime()
	{
		if (_time_startup == 0)
			_time_startup = GetTimeMS();

		long long time = GetTimeMS() - _time_startup;

		return time / 1000.0f;
	}

} // end of namespace