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
#include "log.h"

namespace kge
{
	long long Time::s_time_startup = 0;
	float Time::s_time_delta = 0;
	float Time::s_time_record = -1;
	uint32 Time::s_frame_count = 0;
	uint32 Time::s_frame_record;
	float Time::s_time = 0;
	uint32 Time::s_fps;

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
		if (s_time_startup == 0)
			s_time_startup = GetTimeMS();

		long long time = GetTimeMS() - s_time_startup;

		return time / 1000.0f;
	}

	void Time::Tick()
	{
		float time = Time::GetTime();
		Time::s_time_delta = time - Time::s_time;
		Time::s_time = time;

		if (Time::s_time_record < 0)
		{
			Time::s_time_record = Time::GetTime();
			Time::s_frame_record = Time::GetFrameCount();
		}

		uint32 frame = Time::GetFrameCount();
		if (time - Time::s_time_record >= 1)
		{
			Time::s_fps = frame - Time::s_frame_record;
			Time::s_time_record = time;
			Time::s_frame_record = frame;

			KGE_LOG_DEBUG("fps:%d", Time::GetFPS());
		}

		Time::s_frame_count++;
	}

} // end of namespace