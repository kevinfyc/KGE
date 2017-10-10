//
//  time.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//
// 
#include "math/_math.hpp"

#ifndef __KGE_UTIL_TIME_H__
#define __KGE_UTIL_TIME_H__

namespace kge
{
	class Time
	{
	public:
		static float GetTime();
		static long long GetTimeMS();

		static float GetDeltaTime() { return s_time_delta; }
		static uint32 GetFrameCount() { return s_frame_count; }

		static uint32 GetFPS() { return s_fps; }

		static void Tick();

	private:
		static long long s_time_startup;
		static float s_time_delta;
		static float s_time_record;
		static uint32 s_frame_count;
		static uint32 s_frame_record;
		static float s_time;
		static uint32 s_fps;
	};
} // end of namespace kge

#endif // __KGE_UTIL_TIME_H__